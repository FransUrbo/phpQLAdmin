<?php
// delete a user
// $Id: user_del.php,v 2.42 2005-04-23 08:58:53 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_ezmlm.inc");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);

include($_SESSION["path"]."/header.html");

$url["domain"]		= pql_format_urls($_REQUEST["domain"]);
$url["rootdn"]		= pql_format_urls($_REQUEST["rootdn"]);
$url["user"]		= pql_format_urls($_REQUEST["user"]);
// }}}

// {{{ Get organization name for domain and common name of user
$o = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_O"));
if(!$o) {
	// No 'organization' attribute (or it's not configured - 0)
	// Use the RDN
	$o = $_REQUEST["domain"];
}
$cn = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_CN"));
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Remove user %user% from domain %domain%'), array("domain" => $o, "user" => $cn)); ?></span>
  <br><br>
<?php
// }}}

if(isset($_REQUEST["ok"]) || !pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["rootdn"])) {
	// {{{ Delete a user from the database
	$delete_forwards = (isset($_REQUEST["delete_forwards"]) || pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["rootdn"])) ? true : false;
	$delete_admins   = (isset($_REQUEST["delete_admins"])   || pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["rootdn"])) ? true : false;
	$unsubscribe     = (isset($_REQUEST["unsubscribe"])     || pql_get_define("PQL_CONF_VERIFY_DELETE", $_REQUEST["rootdn"])) ? true : false;

	// {{{ Get the users mail addresses before the object gets deleted
	if($_REQUEST["unsubscribe"]) {
		$mails   = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_MAIL"));
		$aliases = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_MAILALTERNATE"));
		if($mails and !is_array($mails))
		  $mails = array($mails);

		// Combine the two attributes into one array.
		if(is_array($aliases)) {
			foreach($aliases as $alias)
			  $mails[] = $alias;
		}
	}	

// }}}

	// {{{ Delete the user
	if(pql_user_del($_pql, $_REQUEST["domain"], $_REQUEST["user"], $delete_forwards)) {
		$msg = $LANG->_('Successfully removed user') . ": <b>" . $cn . "</b>";
		$rlnb = "&rlnb=1";

		// ----------------------------------------
		// {{{ Remove all administrator/ezmlmAdministrator/controlsAdministrator and seealso
		// attributes that reference this user.
		if($_REQUEST["delete_admins"]) {
			// We're only interested in these attributes
			$attribs = array(pql_get_define("PQL_ATTR_ADMINISTRATOR"),
							 pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS"),
							 pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"),
							 pql_get_define("PQL_ATTR_SEEALSO"));
			pql_replace_values($_pql, $attribs, $_REQUEST["user"]);
		}
		// }}}

		// {{{ Unsubscribe user from all mailinglists (on this host naturaly :)
		if($_REQUEST["unsubscribe"]) {
			// Get all domains, looking for mailing lists
			$domains = pql_get_domains($_pql);
			if(is_array($domains)) {
				foreach($domains as $key => $this_domain) {
					// Get base directory for mails in all domains
					if(($basemaildir = pql_get_attribute($_pql->ldap_linkid, $this_domain, pql_get_define("PQL_ATTR_BASEMAILDIR")))) {
						// Get the lists in this directory
						$user  = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["domain"], pql_get_define("PQL_ATTR_EZMLM_USER"));
						$ezmlm = new ezmlm($user, $basemaildir);
						if(is_array($ezmlm->mailing_lists[0])) {
							// Go through each list in this domain
							foreach($ezmlm->mailing_lists as $number => $data) {
								if(is_array($data["subscriber"])) {
									// Go through each subscriber in this list
									foreach($data["subscriber"] as $subscriber) {
										// Go through each of the users mail addresses
										foreach($mails as $mail) {
											if(eregi($mail, $subscriber)) {
												// The user is subscribed to this list - unsubscribe
												echo "Unsubscribing user from list ".$data["name"]."<br>";
												$ezmlm->unsubscribe($number, $subscriber);
											}
										} // FOREACH address in the subscriber list.
									} // FOREACH subscriber.
								} // IF there are subscribers.
							} // FOREACH mailing lists.
						} // IF there are ezmlm mailing lists.
					} // IF there is $basemaildir.
				} // FOREACH domain.
			} // IF there are domains to check.
		} // IF unsubscribing from all mailing lists.
		// }}}

		// ----------------------------------------
		// Do _something_ to the users mailbox
		switch($_REQUEST["mail_action"]) {
		  case "delete_mail":
		  case "archive_mail":
		  case "donate_mail":
			$msg .= "<br>".$LANG->_('Sorry, no changes have been made to the mailbox. It\'s not implemented in phpQLAdmin yet');
			break;
		}
	} else
	  $msg = $LANG->_('Failed to remove user') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
// }}}

	// {{{ Now it's time to run the special user removal script
	if(pql_get_define("PQL_CONF_SCRIPT_DELETE_USER", $_REQUEST["rootdn"])) {
	  // Setup the environment with the user details
	  putenv("PQL_CONF_DOMAIN=$domain");
	  putenv("PQL_CONF_WEBUSER=".posix_getuid());
	  putenv("PQL_MAIL_ACTION=".$_REQUEST["mail_action"]);
	  
	  if(pql_get_define("PQL_CONF_KRB5_ADMIN_COMMAND_PATH") and 
		 pql_get_define("PQL_CONF_KRB5_REALM") and
		 pql_get_define("PQL_CONF_KRB5_ADMIN_PRINCIPAL") and
		 pql_get_define("PQL_CONF_KRB5_ADMIN_SERVER") and 
		 pql_get_define("PQL_CONF_KRB5_ADMIN_KEYTAB")) {
		putenv("PQL_KADMIN_CMD=".pql_get_define("PQL_CONF_KRB5_ADMIN_COMMAND_PATH")."/kadmin");
		putenv("PQL_KADMIN_REALM=".pql_get_define("PQL_CONF_KRB5_REALM"));
		putenv("PQL_KADMIN_PRINC=".pql_get_define("PQL_CONF_KRB5_ADMIN_PRINCIPAL"));
		putenv("PQL_KADMIN_SERVR=".pql_get_define("PQL_CONF_KRB5_ADMIN_SERVER"));
		putenv("PQL_KADMIN_KEYTB=".pql_get_define("PQL_CONF_KRB5_ADMIN_KEYTAB"));
	  }

	  // Execute the user removal script (0 => show output)
	  if(pql_execute(pql_get_define("PQL_CONF_SCRIPT_DELETE_USER", $_REQUEST["rootdn"]), 0)) {
		echo pql_complete_constant($LANG->_('The %what% removal script failed'),
								   array('what' => $LANG->_('user'))) . "!<br>";
		$msg = urlencode(pql_complete_constant($LANG->_('The %what% removal script failed'),
											   array('what' => $LANG->_('user'))) ."!") . ".&nbsp;<br>";
	  } else {
		echo "<b>" . pql_complete_constant($LANG->_('Successfully executed the %what% removal script'),
										   array('what' => $LANG->_('user'))) . "</b><br>";
		$msg = urlencode(pql_complete_constant($LANG->_('Successfully executed the %what% removal script'),
											   array('what' => $LANG->_('user')))) . ".&nbsp;<br>";
	  }
	}
// }}}
	
	// {{{ Redirect to domain-detail page
	$msg = urlencode($msg);
	$link = "domain_detail.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&msg=$msg$rlnb";

	if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
	  echo "<br>If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
	  die("<b>".$link."<b>");
	} else
	  pql_header($link);
// }}}

	// }}}
} else {
	// {{{ Get confirmation and ask HOW to delete a user
?>
<br>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="GET">
    <input type="hidden" name="user"   value="<?=$url["user"]?>">
    <input type="hidden" name="rootdn" value="<?=$url["rootdn"]?>">
    <input type="hidden" name="domain" value="<?=$url["domain"]?>">

    <span class="title3"><?=$LANG->_('What should we do with forwards to this user')?>?</span><br>
    <input type="checkbox" name="delete_forwards" checked> <?=$LANG->_('Delete all forwards')?><br>
    <input type="checkbox" name="delete_admins" checked> <?=$LANG->_('Remove user from administrator/seeAlso')?><br>
    <input type="checkbox" name="unsubscribe" checked> <?=$LANG->_('Unsubscribe user from all mailing lists')?><br><br>
<?php if(pql_get_define("PQL_CONF_SCRIPT_DELETE_USER", $_REQUEST["rootdn"])) { ?>

    <span class="title3"><?=$LANG->_('What should we do with the users mailbox')?>?</span><br>
    <input type="radio" name="mail_action" value="delete_mail"> <?=$LANG->_('Delete it')?><br>
    <input type="radio" name="mail_action" value="archive_mail" checked> <?=$LANG->_('Archive it')?><br>
    <input type="radio" name="mail_action" value="donate_mail"> <?=$LANG->_('Donate it to another user')?><br><br>
<?php } ?>

    <span class="title2"><?=$LANG->_('Are you really sure')?>?</span>
    <input type="submit" name="ok"   value="<?=$LANG->_('Yes')?>">
    <input type="button" name="back" value="<?=$LANG->_('No')?>" onClick="history.back();">
  </form>
<br>
<?php
	// }}}
} // end of if
?>
</body>
</html>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
