<?php
// attribute for phpQLAdmin/Physical host IP address
// $Id: attrib.hostdhcp.inc,v 2.4 2008-03-19 12:17:50 turbo Exp $

// {{{ attribute_check(void)
function attribute_check() {
  global $error, $LANG;

  $attrib = $_REQUEST["attrib"];
  $ok = 1;

  // is typed in ?
  for($i=0; $i < $_REQUEST[$attrib]; $i++) {
	$var_name = $attrib . "_$i";
	if($_POST[$var_name])  {
	  // Not equal to old value
	  if($_POST[$var_name] == @$_REQUEST["oldvalue"]) {
		$error[$attrib] = $LANG->_('Same as old value');
		return false;
	  }
	}
  }

  if(!$ok) {
	$error[$attrib] = $LANG->_('Missing');
	return false;
  } else
	return true;
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
  global $LANG, $error, $_pql;

  $attrib = $_REQUEST["attrib"];
  if($_REQUEST[$attrib])
    $_REQUEST["oldvalue"] = $_REQUEST[$attrib];
  else
	$_REQUEST[$attrib] = "cn=DHCP,".$_REQUEST["host"];

  // Retreive all DHCP configuration options
  if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
	// We're a super-admin, we can give/take
	// adminstration on ANY branch/domain
	// that exists in the system
	$domains = pql_get_domains();
  } else {
	// We can only give user access to a branch we (the logged in user)
	// have access to. Otherwise, we could 'steal' access to any domain
	// in the system. Only a branch owner/admin can give someone access
	// to the current branch. Am I making myself clear?
	foreach($_SESSION["BASE_DN"] as $dn)  {
	  $dom = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_ADMINISTRATOR"),
								  $_SESSION["USER_DN"], 1);
	  if($dom)
		foreach($dom as $d)
		  $domains[] = $d;
	}
  }

  if(is_array($domains)) {
	$domains = pql_uniq($domains);
	foreach($domains as $domain) {
	  // Get domain part from the DN (Example: 'dc=test,dc=net' => 'test').
	  $d = explode(',', $domain); $d = explode('=', $d[0]); $d = $d[1];

	  $configs = pql_dhcp3_get_configs($domain);
	  if(is_array($configs)) {
		$DATA[$d] = $configs;
	  }
	}
  }
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
<?php if($attrib == sprintf("%s", pql_get_define("PQL_ATTR_IPHOSTNUMBER"))) { ?>
      <th colspan="3" align="left"><?php echo $LANG->_('Change IP address')?></th>
<?php } elseif($attrib == sprintf("%s", pql_get_define("PQL_ATTR_DHCP3_SERVICEDN"))) { ?>
      <th colspan="3" align="left"><?php echo $LANG->_('Change DN')?></th>
<?php } else { ?>
      <th colspan="3" align="left"></th>
<?php } ?>
        <tr class="<?php pql_format_table(); ?>">
<?php if($attrib == sprintf("%s", pql_get_define("PQL_ATTR_IPHOSTNUMBER"))) { ?>
          <td class="title"><?php echo $LANG->_('IP address')?></td>
<?php } elseif($attrib == sprintf("%s", pql_get_define("PQL_ATTR_DHCP3_SERVICEDN"))) { ?>
          <td class="title"><?php echo $LANG->_('DHCP Service')?></td>
<?php } else { ?>
          <td class="title"></td>
<?php } ?>
          <td>
<?php echo pql_format_error_span($error[$attrib]);

	  if(@is_array($DATA)) {
		// {{{ We have DHCP configuration!
		$links = array(); $i =0;
		
		asort($DATA);
		foreach($DATA as $key => $data) {
		  if(is_array($data)) {
			for($j=0; $data[$j]; $j++) {
			  // {{{ Get the common name for this config
			  $tmp = $_pql->get_attribute($data[$j]["dn"], pql_get_define("PQL_ATTR_CN"));
			  if(is_array($tmp))
				$cn = $tmp[0];
			  elseif($tmp)
				$cn = $tmp;
			  else {
				// No CN - use the xx attribute.
				$attr = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]);
				$cn = $_pql->get_attribute($data[$j]["dn"], $attr);
			  }
			  // }}}

			  $new   = array($cn => array($attrib, $i, $data[$j]["dn"]));
			  $links = $links + $new;
			  
			  $i++;
			}
		  } else {
			// {{{ Get the common name for this config
			$tmp = $_pql->get_attribute($data["dn"], pql_get_define("PQL_ATTR_CN"));
			if(is_array($tmp))
			  $cn = $tmp[0];
			elseif($tmp)
			  $cn = $tmp;
			else {
			  // No CN - use the xx attribute.
			  $attr = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"]);
			  $cn = $_pql->get_attribute($data["dn"], $attr);
			}
			// }}}

			$new   = array($cn => array($attrib, $i, $data["dn"]));
			$links = $links + $new;
			
			$i++;
		  }
		  
		  pql_format_tree($key, 0, $links, 0);
		}

		// This an ending for the domain tree
		pql_format_tree_end();
		// }}}
?>
            <input type="hidden" name="<?php echo $attrib?>" value="<?php echo $i?>">
<?php } else {
		// No DHCP configuration!
		echo $LANG->_('There is no configuration to choose from. You will have to go to a\nbranch/domain and create them before you can choose them here');
	  }
?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="action"   value="<?php echo $_REQUEST["action"]?>">
    <input type="hidden" name="attrib"   value="<?php echo $attrib?>">
    <input type="hidden" name="oldvalue" value="<?php echo $_REQUEST["oldvalue"]?>">

    <input type="hidden" name="host"     value="<?php echo urlencode($_REQUEST["host"])?>">
    <input type="hidden" name="view"     value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(void)
function attribute_save($type) {
  global $_pql, $LANG;

  $attrib = $_REQUEST["attrib"];
  
  switch($type) {
  case "modify":
	if($attrib == sprintf("%s", pql_get_define("PQL_ATTR_IPHOSTNUMBER"))) {
	  // {{{ ipHostNumber
	  if(pql_modify_attribute($_REQUEST["host"], $attrib, 1, $_REQUEST[$attrib])) {
		$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									 array('what' => $LANG->_('IP address')));
		if(pql_get_define("PQL_CONF_BIND9_USE") and ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_IPHOSTNUMBER")))) {
		  // TODO: Change a DNS record
		  $x = 'x';
		}
	  } else
		$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									 array('what' => $LANG->_('IP address'))) . ": " . pql_format_error(0);
	  // }}}
	} elseif($attrib == sprintf("%s", pql_get_define("PQL_ATTR_DHCP3_SERVICEDN"))) {
	  // {{{ dhcpServiceDN
	  $entry = array();

	  // {{{ Setup object classes
	  // Get the current object classes of the object.
	  $ocs   = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_OBJECTCLASS"));

	  // Need to lower-case all object classes for the in_array() to work.
	  $new = array();
	  for($i=0; $ocs[$i]; $i++) {
		$new[] = lc($ocs[$i]);
	  }
	  $ocs = $new;

	  // Add: object class dhcpService
	  // TODO: Should we really add the dhcpServer object class!?
	  // Add: object class dhcpServer (if it wasn't added when creating the physical host)
	  $exists_service = 0; $exists_server = 0;
	  for($i=0; $ocs[$i]; $i++) {
		if($ocs[$i] == lc(pql_get_define("PQL_ATTR_DHCP3_SERVICE")))
		  $exists_service = 1;
//		if($ocs[$i] == lc(pql_get_define("PQL_ATTR_DHCP3_SERVER")))
//		  $exists_server = 1;
	  }

	  if($exists_service)
		$ocs[] = pql_get_define("PQL_ATTR_DHCP3_SERVICE");
//	  if($exists_server)
//		$ocs[] = pql_get_define("PQL_ATTR_DHCP3_SERVER");

	  if($exists_service or $exists_server)
		$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = $ocs;
	  // }}}

	  // {{{ Setup the dhcpServiceDN attribute
	  for($i=0; $i < $_REQUEST[$attrib]; $i++) {
		$var_name = $attrib . "_$i";
		if($_POST[$var_name])
		  $entry[$attrib] = $_POST[$var_name];
	  }
	  // }}}

	  // Do the actual modification
	  if($_pql->modify($_REQUEST["host"], $entry, 'attrib.hostdhcp.inc'))
		$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									 array('what' => $LANG->_('DHCP service DN')));
	  else
		$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									 array('what' => $LANG->_('DHCP service DN'))) . ": " . pql_format_error(0);
	  // }}}
	}

	attribute_forward($msg);
	break;
	
  default:
	die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
							  array('type' => $type, 'file' => __FILE__)));
  }
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
