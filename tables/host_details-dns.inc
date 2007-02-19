<?php
// View DNS details for the whole (!) database
// $Id: host_details-dns.inc,v 1.1 2007-02-19 10:54:52 turbo Exp $

// {{{ Setup the host view buttons
if($_REQUEST["ref"] == "global") {
  $buttons = array();
  
  if(pql_get_define("PQL_CONF_HOSTACL_USE")) {
	$new = array('hostacl' => 'Host Control');
	$buttons = $buttons + $new;
  }
  
  if(pql_get_define("PQL_CONF_AUTOMOUNT_USE")) {
	$new = array('automount' => 'Automount Information');
	$buttons = $buttons + $new;
  }
  
  if(pql_get_define("PQL_CONF_CONTROL_USE")) {
	$new = array('mailsrv'   => 'Mailserver Administration');
	$buttons = $buttons + $new;
  }
  
  if(pql_get_define("PQL_CONF_WEBSRV_USE")) {
	$new = array('websrv' => 'Webserver Administration');
	$buttons = $buttons + $new;
  }				 
  
  if(pql_get_define("PQL_CONF_RADIUS_USE")) {
	$new = array('radius' => 'RADIUS Administration');
	$buttons = $buttons + $new;
  } 
  
  if(pql_get_define("PQL_ATTR_BIND9_USE")) {
	$new = array('dns' => 'DNS Administration');
	$buttons = $buttons + $new;
  }
  
  pql_generate_button($buttons, "host=".urlencode($_REQUEST["host"])."&ref=".$_REQUEST["ref"], 'host_detail.php'); echo "    <br>\n";
}
// }}}

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
      <a href="<?=$_SERVER["PHP_SELF"]?>?rootdn=<?=$rootdn?>&domain=<?=$zone_data["@"]["domain_branch_dn"]?>&view=<?=$_REQUEST["view"]?>&dns_domain_name=<?=$zone_name?>&host=<?=$_REQUEST["host"]?>&ref=<?=$_REQUEST["ref"]?>"><img alt="/ <?=$zone_name?> \" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo urlencode($zone_name)?><?=$mark?>"></a>
<?php
	}
?>
      <a href="<?=$_SERVER["PHP_SELF"]?>?view=action&host=<?=$_REQUEST["host"]?>&ref=<?=$_REQUEST["ref"]?>"><img alt="/ <?=$LANG->_('Actions')?> \" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?=$LANG->_('Actions')?>"></a>
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
    <th align="left"><?=$LANG->_('Actions')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td><a href="bind9_del.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&dns_domain_name=<?=$dns_domain_name?>&action=del&type=domain&view=<?=$_REQUEST["view"]?>"><?php echo pql_complete_constant($LANG->_('Remove this %what%'), array('what' => $LANG->_('DNS zone'))); ?></a></td>
      </tr>
<?php if($_SESSION["ALLOW_BRANCH_CREATE"]) { ?>

      <tr class="<?php pql_format_table(); ?>">
        <td><a href="tools/dnszonetemplate.php?rootdn=<?=$_REQUEST["rootdn"]?>&domain=<?=$_REQUEST["domain"]?>&defaultdomain=<?=$dns_domain_name?>&view=<?=$_REQUEST["view"]?>&host=Global&ref=<?=$_REQUEST["ref"]?>"><?=$LANG->_('Create DNS template zone file')?></a></td>
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
