<?php
// add a domain
// $Id: unit_add.php,v 2.15 2004-02-14 14:01:00 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");

include("./header.html");
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create subbranch in domain %domain%'), array('domain' => urldecode($domain))); ?></span>
  <br><br>
<?php

// check if domain exist
if(!pql_domain_exist($_pql, $domain)) {
	echo "Domain &quot;$domain&quot; does not exists";
	exit();
}

if($unit) {
    // Check if unit exist
    if(pql_unit_exist($_pql->ldap_linkid, $domain, $unit)) {
	$msg = urlencode($LANG->_('This sub unit already exists'));
	header("Location: home.php?msg=$msg");
    }
    
    if(pql_unit_add($_pql->ldap_linkid, $domain, $unit)) {
	// Redirect to domain-details
	$msg = urlencode(pql_complete_constant($LANG->_('Sub unit %unit% successfully created'), array("unit" => $unit)));

	if(pql_get_define("PQL_GLOB_AUTO_RELOAD"))
	  $rlnb = "&rlnb=1";

	header("Location: domain_detail.php?rootdn=$rootdn&domain=$domain&msg=$msg$rlnb");
    } else {
	$msg = urlencode($LANG->_('Failed to create the sub unit') . ":&nbsp;" . ldap_error($_pql->ldap_linkid));
	header("Location: home.php?msg=$msg");
    }
} else {
    // Show form
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Create unit')?></th>
        <tr>
          <td class="title"><?=$LANG->_('Unit name')?></td>
          <td><input type="text" name="unit" size="30"></td>
        </tr>

        <tr><td><input type="submit" value="Create"></td></tr>
      </th>
    </table>

    <input type="hidden" name="rootdn" value="<?=urlencode($rootdn)?>">
    <input type="hidden" name="domain" value="<?=urlencode($domain)?>">
  </form>
<?php
}
?>

</body>
</html>
