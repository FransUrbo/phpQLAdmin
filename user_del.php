<?php
// delete a user
// $Id: user_del.php,v 1.3 2002-12-12 21:52:08 turbo Exp $
//
session_start();
require("pql.inc");

include("header.html");
?>
  <span class="title1"><?php echo pql_complete_constant(PQL_USER_DEL_TITLE, array("domain" => $domain, "user" => $user)); ?></span>
  <br><br>
<?php
	if(isset($ok) || PQL_VERIFY_DELETE){
	$delete_forwards = (isset($delete_forwards) || PQL_VERIFY_DELETE) ? true : false;
  	$_pql = new pql($USER_DN, $USER_PASS);

	// delete the user
	if(pql_remove_user($_pql->ldap_linkid, PQL_LDAP_BASEDN, $domain, $user, $delete_forwards)){
		$msg = PQL_USER_DEL_OK . ": <b>" . $user . "</b>";
		$rlnb = "&rlnb=1";
  	} else {
    	$msg = PQL_USER_DEL_FAILED . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
    }

  	// redirect to domain-detail page
	$msg = urlencode($msg);
	header("Location: " . PQL_URI . "domain_detail.php?domain=$domain&msg=$msg$rlnb");
?>
<br>
<form action="<?php echo $PHP_SELF; ?>" method="GET">
	<input type="hidden" name="user" value="<?php echo $user; ?>">
	<input type="hidden" name="domain" value="<?php echo $domain; ?>">
	
	<input type="checkbox" name="delete_forwards" checked> <?php echo PQL_USER_DELETE_FORWARDS; ?><br><br>
	<input type="submit" name="ok" value="<?php echo PQL_YES; ?>">
	<input type="button" name="back" value="<?php echo PQL_NO; ?>" onClick="history.back();">
</form>
<br>
<?php
  } else {
	echo PQL_SURE;
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
