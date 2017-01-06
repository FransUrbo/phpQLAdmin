<?php
// add a domain to a bind9 ldap db
// $Id: bind9_add.php,v 2.35 2007-03-14 12:10:49 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_control.inc");
require($_SESSION["path"]."/include/pql_bind9.inc");

include($_SESSION["path"]."/header.html");
// }}}

if(($_REQUEST["action"] == 'add') and ($_REQUEST["type"] == 'domain')) {
  if(!$_REQUEST["domainname"]) {
	// {{{ action=add && type=domain && !domainname
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create a DNS zone in branch %domain%'), array('domain' => $_REQUEST["domain"])); ?></span>

  <br><br>

  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('DNS domain to branch'))); ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Domain name')?></td>
          <td><input type="text" name="domainname" size="40"></td>
        </tr>
<?php	if(pql_get_define("PQL_CONF_CONTROL_USE") and $_SESSION["ADVANCED_MODE"] and $_SESSION["ALLOW_BRANCH_CREATE"]) {
			// Only offer this if super admin - we might need write access to some other domain/branch (i.e. a domain/branch
			// where the specific DNS domain is located)
?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Update QLC object(s)')?></td>
          <td><input type="checkbox" name="qlc" CHECKED>&nbsp;<?php echo $LANG->_('Yes')?></td>
        </tr>
<?php	} ?>
      </th>
    </table>

    <input type="hidden" name="action"          value="add">
    <input type="hidden" name="type"            value="domain">
    <input type="hidden" name="view"            value="<?php echo $_REQUEST["view"]?>">
    <input type="hidden" name="rootdn"          value="<?php echo $_REQUEST["rootdn"]?>">
    <input type="hidden" name="domain"          value="<?php echo $_REQUEST["domain"]?>">
    <input type="hidden" name="dns_domain_name" value="<?php echo $_REQUEST["dns_domain_name"]?>">
    <input type="submit" value="<?php echo "--&gt;&gt;"; ?>">
  </form>
<?php
// }}}
  } else {
	// {{{ Add bind9 zone
	if(pql_bind9_add_zone($_REQUEST["domain"], $_REQUEST["domainname"])) {
	  $msg = "Successfully added domain ".$_REQUEST["domainname"];

	  // {{{ Update additional domain name(s) and QLC object(s)
	  if($_REQUEST["qlc"] and pql_get_define("PQL_CONF_CONTROL_USE")) {
		// {{{ Fetch old (attrib) values from DB
		unset($entry);
		$oldvalues = $_pql->get_attribute($_REQUEST["domain"], pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME"));
		if($oldvalues) {
		  if(!is_array($oldvalues))
			$oldvalues = array($oldvalues);
		  
		  foreach($oldvalues as $val)
			$entry[pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")][] = $val;
		} else
		  // No previous values
		  $entry[pql_get_define("PQL_ATTR_ADDITIONAL_DOMAINNAME")][] = $_REQUEST["domainname"];
		// }}}

		if(pql_modify_attribute($_REQUEST["domain"], '', '', $entry)) {
		  $msg .= "<br>".pql_complete_constant($LANG->_('Successfully changed %what%'),
											   array('what' => $LANG->_('Additional domain name')));
		  
		  // {{{ Update locals and/or rcptHosts
		  $_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
		  $attribs = array(pql_get_define("PQL_ATTR_LOCALS")    => pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $_REQUEST["rootdn"]),
						   pql_get_define("PQL_ATTR_RCPTHOSTS") => pql_get_define("PQL_CONF_CONTROL_AUTOADDLOCALS", $_REQUEST["rootdn"]));
		  foreach($attribs as $attrib => $autoadd) {
			if($autoadd)
			  $entry = array('', $_REQUEST["domainname"]);
		  }
		  
		  pql_control_update_domains($_REQUEST["rootdn"], $_SESSION["USER_SEARCH_DN_CTR"], '*', $entry);
		  // }}}
		} else
		  $msg .= "<br>".pql_complete_constant($LANG->_('Failed to change %what%'),
											   array('what' => $LANG->_('Additional domain name')))
			. ": " . ldap_error($_pql->ldap_linkid);
	  }
	  // }}}
	} else
	  $msg = "Failed to add domain ".$_REQUEST["domainname"];
	
	if(@$_REQUEST["host"] and $_REQUEST["ref"])
	  $url = "host_detail.php";
	else 
	  $url = "domain_detail.php";
	$url .= "?rootdn=".urlencode($_REQUEST["rootdn"])."&domain=".urlencode($_REQUEST["domain"]);
	$url .= "&dns_domain_name=".$_REQUEST["dns_domain_name"]."&view=".$_REQUEST["view"];
	$url .= "&msg=".urlencode($msg);
	if(@$_REQUEST["host"] and $_REQUEST["ref"])
	  $url .= "&host=".$_REQUEST["host"]."&ref=".$_REQUEST["ref"];
	pql_header($url);
// }}}
  }
} elseif(($_REQUEST["action"] == 'add') and ($_REQUEST["type"] == 'host')) {
  if(!$_REQUEST["hostname"] or !$_REQUEST["record_type"] or !$_REQUEST["dest"]) {
	// {{{ action=add && type=host and (!hostname or !record_type or !dest)
	if(!$_REQUEST["submit"]) {
	  $error_text = array();
	  $error = false;
	  
	  if(!$_REQUEST["record_type"]) {
		$error = true;
		$error_text["record_type"] = $LANG->_('Record type missing');
	  }
	  
	  if(!$_REQUEST["hostname"]) {
		$error = true;
		$error_text["hostname"] = $LANG->_('Hostname missing');
	  }
	  
	  if(!$_REQUEST["dest"]) {
		$error = true;
		$error_text["dest"] = $LANG->_('Resource destination missing');
	  }
	}
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Add a record to domain %domain%'), array('domain' => pql_maybe_idna_decode($_REQUEST["domainname"]))); ?></span>

  <br><br>

  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Add host to domain')?>
        <tr class="title">
          <td></td>
          <td>Source</td>
          <td>Type</td>
          <td>Destination</td>
        </tr>

<?php if($error == true) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"></td>
<?php	if($error_text["hostname"]) { ?>
          <td><?php echo pql_format_error_span($error_text["hostname"]); ?></td>
<?php	} else { ?>
          <td></td>
<?php	}

		if($error_text["record_type"]) {
?>
          <td><?php echo pql_format_error_span($error_text["record_type"]); ?></td>
<?php	} else { ?>
          <td></td>
<?php	}

		if($error_text["dest"]) {
?>
          <td><?php echo pql_format_error_span($error_text["dest"]); ?></td>
<?php	} else { ?>
          <td></td>
<?php	} ?>
        </tr>

<?php } ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">Host name</td>
          <td><input type="text" name="hostname" value="<?php echo $_REQUEST["hostname"]?>" size="15"></td>
          <td>
            <select name="record_type">
              <option value="">Please select record type</option>
              <option value="a" <?php if($_REQUEST["record_type"] == 'a') { echo "SELECTED"; } ?>>A</option>
              <option value="cname" <?php if($_REQUEST["record_type"] == 'cname') { echo "SELECTED"; } ?>>CNAME</option>
              <option value="hinfo" <?php if($_REQUEST["record_type"] == 'hinfo') { echo "SELECTED"; } ?>>HINFO</option>
              <option value="mx" <?php if($_REQUEST["record_type"] == 'mx') { echo "SELECTED"; } ?>>MX</option>
              <option value="ns" <?php if($_REQUEST["record_type"] == 'ns') { echo "SELECTED"; } ?>>NS</option>
              <option value="ptr" <?php if($_REQUEST["record_type"] == 'ptr') { echo "SELECTED"; } ?>>PTR</option>
              <option value="txt" <?php if($_REQUEST["record_type"] == 'txt') { echo "SELECTED"; } ?>>TXT</option>
            </select>
          </td>
          <td><input type="text" name="dest" value="<?php echo $_REQUEST["dest"]?>" size="20"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="4">
            <img src="images/info.png" width="16" height="16" alt="" border="0">
            <?php echo $LANG->_('\bNOTE\B: If destination is not IP and not within current zone, it \umust\U end with dot!')?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"          value="0">
    <input type="hidden" name="action"          value="add">
    <input type="hidden" name="type"            value="host">
    <input type="hidden" name="view"            value="<?php echo $_REQUEST["view"]?>">
    <input type="hidden" name="domain"          value="<?php echo $_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn"          value="<?php echo $_REQUEST["rootdn"]?>">
    <input type="hidden" name="domainname"      value="<?php echo $_REQUEST["domainname"]?>">
    <input type="hidden" name="dns_domain_name" value="<?php echo $_REQUEST["dns_domain_name"]?>">
<?php if($_REQUEST["host"] and $_REQUEST["ref"]) { ?> 
    <input type="hidden" name="host"            value="<?php echo $_REQUEST["host"]?>">
    <input type="hidden" name="ref"             value="<?php echo $_REQUEST["ref"]?>">
<?php } ?>
    <br>
    <input type="submit" value="Save">
  </form>
<?php
// }}}
  } else {
	// {{{ Add bind9 host
	if($_REQUEST["record_type"] == "ptr") {
	  // {{{ Special circumstances - it's a PTR.
	  // Check if "dest" is IP
	  $num = "(\\d|[1-9]\\d|1\\d\\d|2[0-4]\\d|25[0-5])";
	  if(preg_match("/^$num\\.$num\\.$num\\.$num$/", $_REQUEST['dest'])) {
		$ip = $_REQUEST["dest"];
		$hn = $_REQUEST["hostname"];
	  } else {
		$hn = $_REQUEST["dest"];
		$ip = $_REQUEST["hostname"];
	  }
	  
	  // Reverse the hostname ('192.168.156.1').
	  $tmp  = explode('\.', $ip);
	  $count = count($tmp);
	  if(empty($tmp[$count-1]))
		$count = $count - 2;
	  else
		$count = $count - 1;

	  for($i=$count; $i >= 0; $i--) {
		$rev .= $tmp[$i];
		if($tmp[$i-1])
		  $rev .= ".";
	  }
	  // rev='4.156.168.192'
	  
	  // Extract the zone part from the zone/domain name ('168.192.in-addr.arpa').
	  $zone = preg_replace('/\.in-addr\.arpa/', '', $_REQUEST["domainname"]);
	  $zone = preg_replace('/\./', '\\\.', $zone, -1); // Just so that next regexp doesn't catch the dot.
	  // zone='168\.192'
	  
	  // Remove the zone ('.168.192') from the reverse ('4.156.168.192') => '4.156'.
	  $host = preg_replace("/\.$zone/", '', $rev);
	  // host='4.156'
	  
	  $entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")]	= $host;
// }}}
	} else
	  $entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")]	= pql_maybe_idna_encode($_REQUEST["hostname"]);
	$entry[pql_get_define("PQL_ATTR_ZONENAME")]			= $_REQUEST["domainname"];
	$entry[pql_get_define("PQL_ATTR_DNSTTL")]			= pql_bind9_get_ttl($_pql->ldap_linkid, $_REQUEST["domainname"]);
	switch($_REQUEST["record_type"]) {
	case "a":
	  $entry[pql_get_define("PQL_ATTR_ARECORD")]		= pql_maybe_idna_encode($_REQUEST["dest"]);
	  break;
	case "cname":
	  $entry[pql_get_define("PQL_ATTR_CNAMERECORD")]	= pql_maybe_idna_encode($_REQUEST["dest"]);
	  break;
	case "hinfo":
	  $entry[pql_get_define("PQL_ATTR_HINFORECORD")]	= $_REQUEST["dest"];
	  break;
	case "mx":
	  $entry[pql_get_define("PQL_ATTR_MXRECORD")]		= pql_maybe_idna_encode($_REQUEST["dest"]);
	  break;
	case "ns":
	  $entry[pql_get_define("PQL_ATTR_NSRECORD")]		= pql_maybe_idna_encode($_REQUEST["dest"]);
	  break;
	case "ptr":
	  $entry[pql_get_define("PQL_ATTR_PTRRECORD")]		= pql_maybe_idna_encode($hn);
	  break;
	case "txt":
	  $entry[pql_get_define("PQL_ATTR_TXTRECORD")]		= pql_maybe_idna_encode($_REQUEST["dest"]);
	  unset($entry[pql_get_define("PQL_ATTR_DNSTTL")]);
	  break;
	}
	
	$dn = pql_bind9_add_host($_REQUEST["domain"], $entry);
	if($dn) {
	  $msg = "Successfully added host <u>".$_REQUEST["hostname"].".".pql_maybe_idna_decode($_REQUEST["domainname"])."</u>";

	  // {{{ Try to figure out the SOA DN here...
	  $dn_parts = explode(',', $dn);
	  for($i=1; $i < count($dn_parts); $i++) {
		$newdn .= $dn_parts[$i];
		if($dn_parts[$i+1])
		  $newdn .= ',';
	  }
	  $dn_soa = 'dNSTTL=3600+relativeDomainName=@,'.$newdn;
	  
	  // Verify that this object realy exists
	  if(!$_pql->get_dn($dn_soa, 'objectClass=*', 'BASE')) {
		// It doesn't! Try without the 'dNSTTL=3600+' part...
		if($_pql->get_dn('relativeDomainName=@,'.$newdn, 'objectClass=*', 'BASE')) {
		  // That worked better, use it...
		  $dn_soa = 'relativeDomainName=@,'.$newdn;
		}
	  }
	  // }}}
	
	  if(!pql_bind9_update_serial($dn_soa))
		die("failed to update SOA serial number");
	} else
	  $msg = "Failed to add ".$_REQUEST["hostname"]." to ".$_REQUEST["domainname"];
	
	$msg = urlencode($msg);

	if(@$_REQUEST["host"] and $_REQUEST["ref"])
	  $url = "host_detail.php";
	else 
	  $url = "domain_detail.php";
	$url .= "?rootdn=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["domain"]."&view=".$_REQUEST["view"];
	$url .= "&dns_domain_name=".$_REQUEST["dns_domain_name"]."&msg=$msg";
	if(@$_REQUEST["host"] and $_REQUEST["ref"])
	  $url .= "&host=".$_REQUEST["host"]."&ref=".$_REQUEST["ref"];
	pql_header($url);
// }}}
  }
}
?>
</body>
</html>
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
