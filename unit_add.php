<?php
// add a domain
// $Id: unit_add.php,v 2.21.4.1 2005-02-12 05:19:13 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require($_SESSION["path"]."/include/pql_control.inc");

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);
$url["unit"]   = pql_format_urls($_REQUEST["unit"]);

include($_SESSION["path"]."/header.html");
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create subbranch in domain %domain%'),
							array('domain' => pql_maybe_decode($_REQUEST["domain"]))); ?></span>
  <br><br>
<?php

// check if domain exist
if(!pql_get_dn($_pql->ldap_linkid, $_REQUEST["domain"], '(objectclass=*)', 'BASE')) {
	echo "Domain &quot;".$_REQUEST["domain"]."&quot; does not exists";
	exit();
}

if($_REQUEST["unit"]) {
    // Check if unit exist
    if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $_REQUEST["rootdn"]) == "dc")
      $filter = "(&(dc=".$_REQUEST["unit"].")(objectclass=domain))";
    else
      $filter = "(&(ou=".$_REQUEST["unit"].")(objectclass=organizationalUnit))";
    if(pql_get_dn($_pql->ldap_linkid, $_REQUEST["domain"], $filter, 'ONELEVEL')) {
	$msg = urlencode($LANG->_('This sub unit already exists'));
	header("Location: home.php?msg=$msg");
    }
    
    if(pql_unit_add($_pql->ldap_linkid, $_REQUEST["domain"], $_REQUEST["unit"])) {
	// Redirect to domain-details
	$msg = urlencode(pql_complete_constant($LANG->_('Sub unit %unit% successfully created'), array("unit" => $_REQUEST["unit"])));

	if(pql_get_define("PQL_CONF_AUTO_RELOAD"))
	  $rlnb = "&rlnb=1";

	header("Location: domain_detail.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"]."&msg=$msg$rlnb");
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

    <input type="hidden" name="rootdn" value="<?=$url["rootdn"]?>">
    <input type="hidden" name="domain" value="<?=$url["domain"]?>">
  </form>
<?php
}
?>

</body>
</html>
