<?php
// delete a user
// user_del.php,v 1.3 2002/12/12 21:52:08 turbo Exp
//
session_start();
require("./include/pql_config.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

include("./header.html");

// Get organization name for domain and common name of user
$o = pql_get_domain_value($_pql, $domain, 'o');
$cn = pql_get_userattribute($_pql->ldap_linkid, $user, 'cn');
$cn = $cn[0];
?>
  <span class="title1"><?php echo pql_complete_constant(PQL_LANG_USER_DEL_TITLE, array("domain" => $o, "user" => $cn)); ?></span>
  <br><br>
<?php
if(isset($ok) || !PQL_CONF_VERIFY_DELETE) {
	$delete_forwards = (isset($delete_forwards) || PQL_CONF_VERIFY_DELETE) ? true : false;
	
	// delete the user
	if(pql_user_del($_pql, $domain, $user, $delete_forwards)){
		$msg = PQL_LANG_USER_DEL_OK . ": <b>" . $cn . "</b>";
		$rlnb = "&rlnb=1";
	} else {
		$msg = PQL_LANG_USER_DEL_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
	}
	
	// redirect to domain-detail page
	$msg = urlencode($msg);
	header("Location: " . PQL_CONF_URI . "domain_detail.php?domain=$domain&msg=$msg$rlnb");
} else {
	echo PQL_LANG_SURE;
?>
<br>
  <form action="<?php echo $PHP_SELF; ?>" method="GET">
    <input type="hidden" name="user" value="<?php echo $user; ?>">
    <input type="hidden" name="domain" value="<?php echo $domain; ?>">
        
    <input type="checkbox" name="delete_forwards" checked> <?php echo PQL_LANG_USER_DELETE_FORWARDS; ?><br><br>
    <input type="submit" name="ok" value="<?php echo PQL_LANG_YES; ?>">
    <input type="button" name="back" value="<?php echo PQL_LANG_NO; ?>" onClick="history.back();">
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
