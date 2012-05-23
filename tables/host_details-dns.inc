<?php
// View DNS details for the whole (!) database
// $Id: host_details-dns.inc,v 1.2 2007-02-19 19:25:37 turbo Exp $

// {{{ ---------------- GET THE DOMAINS/BRANCHES
if($_SESSION["ALLOW_BRANCH_CREATE"]) {
  // This is a 'super-admin'. Should be able to read EVERYTHING!
  $domains = pql_get_domains();
} else {
  // {{{ Get ALL domains we have access to.
  //     I.e., all DN's with 'administrator: USER_DN'
  foreach($_SESSION["BASE_DN"] as $dn)  {
	$dom = $_pql->get_dn($dn, pql_get_define("PQL_ATTR_ADMINISTRATOR")."=".$_SESSION["USER_DN"]);
	if($dom) {
	  foreach($dom as $d) {
		$domains[] = $d;
	  }
	}
  }
  // }}}
}
// }}}

// {{{ Setup session and retreive zones
require($_SESSION["path"]."/include/pql_bind9.inc");

$zones = array();
foreach($domains as $domain)  {
  $zone = pql_bind9_get_zone($domain);
  if(is_array($zone)) {
	foreach($zone as $key => $data) {
	  $data["@"]["domain_branch_dn"] = $domain;
	  $zones[$key] = $data;
	}
  }
}
// }}}

if(is_array($zones)) {
  if($_REQUEST["ref"] == 'zone') {
	// Called with specific zone
	$dns_domain_name = $_REQUEST["dns_domain_name"];
  } else {
?>
  <table cellspacing="0" border="0" width="50%" cellpadding="0">
    <th colspan="3" align="left">
<?php
	// Go through all the zones...
	foreach($zones as $zone_name => $zone_data) {
	  // {{{ Use the first zone as default
	  // ie, if we're not called with a specific zone to show
	  if(!@$_REQUEST["dns_domain_name"] and !@$dns_domain_name) {
		$dns_domain_name = $zone_name;
		$_REQUEST["dns_domain_name"] = $dns_domain_name;
	  } elseif(@$_REQUEST["dns_domain_name"]) {
		$dns_domain_name = $_REQUEST["dns_domain_name"];
	  }
	  
	  if($zone_name == $_REQUEST["dns_domain_name"]) {
		$mark = "&mark=yes";
	  } else {
		unset($mark);
	  }
// }}}

	  $rootdn = pql_get_rootdn($zone_data["@"]["domain_branch_dn"], "host_details-dns.inc");
?>
      <a href="<?php echo $_SERVER["PHP_SELF"]?>?rootdn=<?php echo $rootdn?>&domain=<?php echo $zone_data["@"]["domain_branch_dn"]?>&view=<?php echo $_REQUEST["view"]?>&dns_domain_name=<?php echo $zone_name?>&host=<?php echo $_REQUEST["host"]?>&ref=<?php echo $_REQUEST["ref"]?>"><img alt="/ <?php echo $zone_name?> \" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo urlencode($zone_name)?><?php echo $mark?>"></a>
<?php
	}
?>
      <a href="<?php echo $_SERVER["PHP_SELF"]?>?view=action&host=<?php echo $_REQUEST["host"]?>&ref=<?php echo $_REQUEST["ref"]?>"><img alt="/ <?php echo $LANG->_('Actions')?> \" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo $LANG->_('Actions')?>"></a>
    </th>
  </table>

  <br>

<?php
}

$zone_data = $zones[$dns_domain_name];
$rootdn = pql_get_rootdn($zone_data["@"]["domain_branch_dn"], "host_details-dns.inc");
$_REQUEST["rootdn"] = $rootdn;
$_REQUEST["domain"] = $zone_data["@"]["domain_branch_dn"];

$additional_uri = "&host=".$_REQUEST["host"]."&ref=".$_REQUEST["ref"];

require($_SESSION["path"]."/tables/domain_details-dnszone_soa.inc");
echo "<br>";
require($_SESSION["path"]."/tables/domain_details-dnszone_data.inc");
?>
  <br>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?php echo $LANG->_('Actions')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="bind9_del.php?rootdn=<?php echo $_REQUEST["rootdn"]?>&domain=<?php echo $_REQUEST["domain"]?>&dns_domain_name=<?php echo $dns_domain_name?>&action=del&type=domain&view=<?php echo $_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Remove this %what%'), array('what' => $LANG->_('DNS zone'))); ?></a></td>
      </tr>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="tools/dnszonetemplate.php?rootdn=<?php echo $_REQUEST["rootdn"]?>&domain=<?php echo $_REQUEST["domain"]?>&defaultdomain=<?php echo $dns_domain_name?>&view=<?php echo $_REQUEST["view"]?>&host=Global&ref=<?php echo $_REQUEST["ref"]?>"><?php echo $LANG->_('Create DNS template zone file')?></a></td>
      </tr>
<?php } ?>
    </th>
  </table>
<?php
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
