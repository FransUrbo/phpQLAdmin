<?php
// delete a user
// user_del.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");

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

	// delete the user
	if(pql_user_del($_pql, $domain, $user, $delete_forwards)) {
		$msg = $LANG->_('Successfully removed user') . ": <b>" . $cn . "</b>";
		$rlnb = "&rlnb=1";

		if($delete_admins)
		  // Remove all administrator/seealso attributes that reference this user
		  pql_replace_admins($_pql, $user, '');

//		if($unsubscribe)
//		  // TODO: Unsubscribe user from all mailinglists (on this host naturaly :)
	} else
	  $msg = $LANG->_('Failed to remove user') . ":&nbsp;" . ldap_error($_pql->ldap_linkid);

	switch($mail) {
	  case "delete_mail":
	  case "archive_mail":
	  case "donate_mail":
		$msg .= "<br>".$LANG->_('Sorry, no changes have been made to the mailbox. It\'s not implemented in phpQLAdmin yet');
		break;
	}
	
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
<?php if(0) { ?>
    <input type="checkbox" name="unsubscribe" checked> <?=$LANG->_('Unsubscribe user from all mailing lists')?><br><br>

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
