<?php
// delete a user
// $Id: user_del.php,v 2.23 2003-11-14 11:55:52 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_ezmlm.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

include("./header.html");

// Get organization name for domain and common name of user
$o = pql_get_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_O"));
if(!$o) {
	// No 'organization' attribute (or it's not configured - 0)
	// Use the RDN
	$o = $domain;
}
$cn = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_CN")); $cn = $cn[0];
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Remove user %user% from domain %domain%'), array("domain" => $o, "user" => $cn)); ?></span>
  <br><br>
<?php
if(isset($ok) || !pql_get_define("PQL_CONF_VERIFY_DELETE", $rootdn)) {
	$delete_forwards = (isset($delete_forwards) || pql_get_define("PQL_CONF_VERIFY_DELETE", $rootdn)) ? true : false;
	$delete_admins   = (isset($delete_admins)   || pql_get_define("PQL_CONF_VERIFY_DELETE", $rootdn)) ? true : false;
	$unsubscribe     = (isset($unsubscribe)     || pql_get_define("PQL_CONF_VERIFY_DELETE", $rootdn)) ? true : false;

	if($unsubscribe) {
		// We want to unsubscribe user from (all) mailing list(s).
		// Get the users mail addresses before the object gets deleted
		$email   = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_MAIL"));
		$aliases = pql_get_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_MAILALTERNATE"));

		// Combine the two attributes into one array.
		$mails[] = $email[0];
		foreach($aliases as $alias)
		  $mails[] = $alias;
	}	

	// delete the user
	if(pql_user_del($_pql, $domain, $user, $delete_forwards)) {
		$msg = $LANG->_('Successfully removed user') . ": <b>" . $cn . "</b>";
		$rlnb = "&rlnb=1";

		// ----------------------------------------
		// Remove all administrator/ezmlmAdministrator/controlsAdministrator and seealso
		// attributes that reference this user.
		if($delete_admins)
		  pql_replace_admins($_pql, $user, '');

		// ----------------------------------------
		// Unsubscribe user from all mailinglists (on this host naturaly :)
		if($unsubscribe) {
			// Get all domains, looking for mailing lists
			$domains = pql_get_domains($_pql);
			if(is_array($domains)) {
				asort($domains);
				foreach($domains as $key => $domain) {
					// Get base directory for mails in all domains
					if(($basemaildir = pql_get_domain_value($_pql, $domain, pql_get_define("PQL_GLOB_ATTR_BASEMAILDIR")))) {
						// Get the lists in this directory
						$ezmlm = new ezmlm(pql_get_define("PQL_GLOB_EZMLM_USER"), $basemaildir);
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
										}
									}
								}
							}
						}
					}
				}
			}
		}

		// ----------------------------------------
		// Do _something_ to the users mailbox
		switch($mail) {
		  case "delete_mail":
		  case "archive_mail":
		  case "donate_mail":
			$msg .= "<br>".$LANG->_('Sorry, no changes have been made to the mailbox. It\'s not implemented in phpQLAdmin yet');
			break;
		}
	} else
	  $msg = $LANG->_('Failed to remove user') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);

	// redirect to domain-detail page
	$msg = urlencode($msg);
	header("Location: " . pql_get_define("PQL_GLOB_URI") . "domain_detail.php?rootdn=$rootdn&domain=$domain&view=$view&msg=$msg$rlnb");
} else {
?>
<br>
  <form action="<?=$PHP_SELF?>" method="GET">
    <input type="hidden" name="user"   value="<?=$user?>">
    <input type="hidden" name="rootdn" value="<?=$rootdn?>">
    <input type="hidden" name="domain" value="<?=$domain?>">
        
    <span class="title3"><?=$LANG->_('What should we do with forwards to this user')?>?</span><br>
    <input type="checkbox" name="delete_forwards" checked> <?=$LANG->_('Delete all forwards')?><br>
    <input type="checkbox" name="delete_admins" checked> <?=$LANG->_('Remove user from administrator/seeAlso')?><br>
    <input type="checkbox" name="unsubscribe" checked> <?=$LANG->_('Unsubscribe user from all mailing lists')?><br><br>
<?php if(0) { ?>

    <span class="title3"><?=$LANG->_('What should we do with the users mailbox')?>?</span><br>
    <input type="radio" name="mail" value="delete_mail"> <?=$LANG->_('Delete it')?><br>
    <input type="radio" name="mail" value="archive_mail" checked> <?=$LANG->_('Archive it')?><br>
    <input type="radio" name="mail" value="donate_mail"> <?=$LANG->_('Donate it to another user')?><br><br>
<?php } ?>

    <span class="title2"><?=$LANG->_('Are you really sure')?>?</span>
    <input type="hidden" name="view" value="<?=$view?>">
    <input type="submit" name="ok"   value="<?=$LANG->_('Yes')?>">
    <input type="button" name="back" value="<?=$LANG->_('No')?>" onClick="history.back();">
  </form>
<br>
<?php
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
