<?php
// attribute plugin for
// BIND9 DNS zone
// $Id: attrib.dnszone.inc,v 2.15.2.2 2005-02-16 12:50:15 turbo Exp $

// {{{ attribute_translate_type(attrib)
function attribute_translate_type($attrib) {
  // Translate the TYPE to dNSZone objectclass attribute
  switch($_REQUEST["attrib"]) {
	case "a":
	case "host":
	  return 'relativeDomainName';
	case "ttl":
	  return 'dNSTTL';
	case "ns":
	  return 'nSRecord';
	case "mx":
	  return 'mXRecord';
	case "cname":
	  return 'cNAMERecord';
	case "srv":
	  return 'sRVRecord';
	case "txt":
	  return 'tXTRecord';
	default:
	  return $_REQUEST["attrib"];
  }
}
// }}}

// {{{ attribute_check(void)
function attribute_check() {
	// dummy function
	return true;
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
	global $_pql, $LANG;

	$type = $_REQUEST["type"];
	$type_value = $_REQUEST[$type];

	if($_REQUEST["attrib"])
	  $attrib = attribute_translate_type($_REQUEST["attrib"]);

	if($_REQUEST["action"] != 'add') {
		// Get old value
		$dn = urldecode($_REQUEST["rdn"]);
		$_REQUEST["oldvalue"] = pql_get_attribute($_pql->ldap_linkid, $dn, $attrib);
		if(is_array($_REQUEST["oldvalue"])) {
		  for($i=0; $_REQUEST["oldvalue"][$i]; $i++) {
			if(eregi($type_value, $_REQUEST["oldvalue"][$i]))
			  $oldvalue = $_REQUEST["oldvalue"][$i];
		  }

		  $_REQUEST["oldvalue"] = $oldvalue;
		}
	}

	if((lc($attrib) == 'mxrecord')) {
		if($_REQUEST["oldvalue"]) {
			$tmp  = split(' ', $_REQUEST["oldvalue"]);
			$prio = $tmp[0];
			$_REQUEST["oldvalue"] = $tmp[1];
		} else
		  $prio = '10';
	}

	// relativeDomainName zoneName
	$host   = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME"));
	if(is_array($host))
	  $host = $host[0];
	$domain = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_ZONENAME"));
	if(is_array($domain))
	  $domain = $domain[0];

	if($host != "@")
	  $host = $host.".".$domain;
?>
    <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="0" cellpadding="3" border="0">
        <th colspan="3" align="left"><?php
	   if(lc($attrib) == pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")) {
		 echo pql_complete_constant($LANG->_("Rename DNS host %host%"), array('host' => $host));
	   } elseif($host != "@") {
		 echo pql_complete_constant($LANG->_('Change Bind9 value for %host%'), array('host' => $host));
	   } else {
		 echo pql_complete_constant($LANG->_("Change Bind9 SOA value for %domain%"), array('domain' => $domain));
	   }
?>
        </th>
<?php	if(lc($attrib) == 'mxrecord') { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_('MX Priority')?>:</td>
            <td><input type="text" name="priority" value="<?=$prio?>" size="20"></td>
            <td></td>
          </tr>

<?php	} ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?php echo pql_complete_constant($LANG->_('Change attribute %attribute%'), array('attribute' => $attrib)); ?></td>
            <td><input type="text" name="newvalue" value="<?=$_REQUEST["oldvalue"]?>" size="20"></td>
<?php	if(lc($attrib) == 'dnsttl') { ?>
            <td>
              <select name="time">
                <option value="seconds" SELECTED><?=$LANG->_('Seconds')?></option>
                <option value="minutes"><?=$LANG->_('Minutes')?></option>
                <option value="hours"><?=$LANG->_('Hours')?></option>
                <option value="days"><?=$LANG->_('Days')?></option>
                <option value="weeks"><?=$LANG->_('Weeks')?></option>
                <option value="months"><?=$LANG->_('Months')?></option>
              </select>
            </td>
<?php	} else { ?>
            <td></td>
<?php	} ?>
          </tr>

<?php	if(lc($attrib) == 'mxrecord') { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title" align="right">
              <img src="images/info.png" width="16" height="16" alt="" border="0" align="right">
            </td>
            <td><?=$LANG->_("If this is an FQDN not in this zone,<br>don't forget the trailing dot!")?></td>
            <td></td>
          </tr>
<?php	} elseif((lc($attrib) != 'nsrecord') and (lc($attrib) != 'dnsttl') and (lc($attrib) != pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME"))) { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title">New record type</td>
            <td>
              <select name="type">
                <option value="a" <?php     if($_REQUEST[$attrib] == 'a')     { echo "SELECTED"; } ?>>A</option>
                <option value="cname" <?php if($_REQUEST[$attrib] == 'cname') { echo "SELECTED"; } ?>>CNAME</option>
                <option value="hinfo" <?php if($_REQUEST[$attrib] == 'hinfo') { echo "SELECTED"; } ?>>HINFO</option>
                <option value="mx" <?php    if($_REQUEST[$attrib] == 'mx')    { echo "SELECTED"; } ?>>MX</option>
                <option value="ns" <?php    if($_REQUEST[$attrib] == 'ns')    { echo "SELECTED"; } ?>>NS</option>
              </select>
            </td>
            <td></td>
<?php   } ?>
        </th>
      </table>

      <input type="hidden" name="submit"          value="1">
<?php	if($action != 'add') {
			if((lc($attrib) == 'mxrecord')) {
?>
      <input type="hidden" name="oldvalue"        value="<?=$prio." ".$_REQUEST["oldvalue"]?>">
<?php		} else { ?>
      <input type="hidden" name="oldvalue"        value="<?=$_REQUEST["oldvalue"]?>">
<?php		} ?>
      <input type="hidden" name="type"            value="<?=$type?>">
<?php	} ?>
      <input type="hidden" name="domain"          value="<?=urlencode($_REQUEST["domain"])?>">
      <input type="hidden" name="rootdn"          value="<?=urlencode($_REQUEST["rootdn"])?>">
      <input type="hidden" name="view"            value="<?=$_REQUEST["view"]?>">
<?php	if(lc($attrib) == pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")) { ?>
      <input type="hidden" name="action"          value="rename">
<?php	} else { ?>
      <input type="hidden" name="action"          value="<?=$_REQUEST["action"]?>">
<?php	} ?>
      <input type="hidden" name="rdn"             value="<?=urlencode($_REQUEST["rdn"])?>">
      <input type="hidden" name="dns_domain_name" value="<?=$_REQUEST["dns_domain_name"]?>">
      <br>
      <input type="submit" value="<?=$LANG->_('Save')?>">
    </form>
<?php
}
// }}}

// {{{ attribute_save(action)
function attribute_save($action) {
	global $_pql, $LANG;
	$success = 0;

	$dn = urldecode($_REQUEST["rdn"]);

	if($_REQUEST["type"])
	  $attrib = attribute_translate_type($_REQUEST["type"]);

	if($_REQUEST["newvalue"]) {
	  $newvalue = $_REQUEST["newvalue"];
	  
	  if(@(lc($attrib) == 'mxrecord') && $_REQUEST["priority"])
		$newvalue = $_REQUEST["priority"] . " " . $newvalue;
	}

	// {{{ if($attrib == 'dnsttl')
	if((lc($attrib) == 'dnsttl') && $_REQUEST["time"]) {
	  if($_REQUEST["time"] == "minutes")
		$newvalue = $value * 60;
	  elseif($_REQUEST["time"] == "hours")
		$newvalue = $value * 60 * 60;
	  elseif($_REQUEST["time"] == "days")
		$newvalue = $value * 60 * 60 * 24;
	  elseif($_REQUEST["time"] == "weeks")
		$newvalue = $value * 60 * 60 * 24 * 7;
	  elseif($_REQUEST["time"] == "months")
		$newvalue = $value * 60 * 60 * 24 * 7 * 4;
	}
	// }}}

    switch($action) {
	  case "add":
		// {{{ add
		// Get old value
		$oldvalues = pql_get_attribute($_pql->ldap_linkid, $dn, $attrib);
		if($oldvalues and !is_array($oldvalues))
		  $oldvalues = array($oldvalues);

		// Add the old values to the array
		for($i=0; $oldvalues[$i]; $i++)
		  $entry[$attrib][] = $oldvalues[$i];

		// Add the new value to the array
		$entry[$attrib][] = $value;

		if(pql_modify_attribute($_pql->ldap_linkid, $dn, '', '', $entry)) {
		  $success = 1;
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $attrib)).".";
		} else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		break;
		// }}}

	  case "modify":
		// {{{ mod

		// {{{ if($attrib == relativeDomainName)
		if($attrib == pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")) {
			// We must also change the RDN!
			$dnattrs = explode(",", urldecode($dn));

			$new_dn  = pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")."=$newvalue";
			for($i=1; $dnattrs[$i]; $i++) {
				$parent .= $dnattrs[$i];
				if($dnattrs[$i+1])
				  $parent .= ",";
			}

			if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
				echo "Renaming<br>'<b>$dn</b>'<br>to<br>'<b>$new_dn,$parent</b>'<br>";
				$dn = $new_dn.','.$parent;
			} else {
				if(ldap_rename($_pql->ldap_linkid, $dn, $new_dn, $parent, true))
				  $dn = $new_dn.','.$parent;
				else {
					pql_format_error(1);
					die("failed rename");
				}
			}
		}
		// }}}

	  	// {{{ if(oldtype != newtype)
		if($oldtype != $newtype) {
		  // Remove the 'oldtype' value
		  if(!pql_modify_attribute($_pql->ldap_linkid, $dn, $_REQUEST["oldtype"], '', ''))
			// This shouldn't really be possible, but until I find out that/if I'm wrong, I'll just die here!
			die("Failed to remove the old attribute ".$_REQUEST["oldtype"]." with value ".$_REQUEST["oldvalue"]);
		}
		// }}}

		if((lc($attrib) == 'mxrecord') or (lc($attrib) == 'nsrecord'))
		  $oldvalue = $_REQUEST["oldvalue"];
		else
		  $oldvalue = 1;

		if(pql_modify_attribute($_pql->ldap_linkid, $dn, $attrib, $oldvalue, $newvalue)) {
		  $success = 1;
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $attrib));
		} else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		break;
		// }}}

	  case "rename":
		// {{{ ren
		$rdn_old = urldecode($_REQUEST["rdn"]);

		$tmp     = explode(',', $rdn_old);
		$attrib  = explode('=', $tmp[0]); $attrib=$attrib[0];

		// {{{ Generate parent DN
		$count   = count($tmp);
		for($i=1; $i <= $count; $i++) {
		  $parent .= $tmp[$i];
		  if($tmp[$i+1])
			$parent .= ",";
		}
		// }}}

		// Generate new RDN
		$rdn_new = "$attrib=".$_REQUEST["newvalue"];

		if(pql_write_ren($_pql->ldap_linkid, $rdn_old, $parent, $rdn_new))
		  $msg = pql_complete_constant($LANG->_('Successfully renamed RDN %old% to %new%'),
									   array('old' => $rdn_old,
											 'new' => $rdn_new));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to rename RDN %old%'),
									   array('old' => $rdn_old)) . ":&nbsp;" . ldap_error($_pql->ldap_linkid);
		break;
		// }}}

	  case "del":
		// {{{ del
		if($_REQUEST["type"] and $_REQUEST["oldvalue"]) {
			// Get old value
			$oldvalues = pql_get_attribute($_pql->ldap_linkid, $dn, $attrib);
			if($oldvalues and !is_array($oldvalues))
			$oldvalues = array($oldvalues);

			for($i=0; $oldvalues[$i]; $i++) {
				// Filter out the value to delete
				if($oldvalues[$i] != $_REQUEST["oldvalue"])
				  $entry[$attrib][] = $oldvalues[$i];
			}

			if(pql_modify_attribute($_pql->ldap_linkid, $dn, '', '', $entry)) {
			  $success = 1;
			  $msg = pql_complete_constant($LANG->_('Successfully removed %value%'),
										   array('value' => $_REQUEST["oldvalue"]));
			} else
			  $msg = pql_complete_constant($LANG->_('Failed to remove %value%'),
										   array('value' => $_REQUEST["oldvalue"])) . ": " . ldap_error($_pql->ldap_linkid);
		} else {
		  // Delete a whole object
		  if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
			$success = 1;
			echo "Deleting a whole object: '$dn'";
		  } else {
			if(ldap_delete($_pql->ldap_linkid, $dn))
			  $success = 1;
			else
			  pql_format_error(1);
		  }

		  // Extact the host part of the DN
		  $host = split(',', $dn); $host = split('=', $host[0]); $host = $host[1];

		  if($success) {
			// {{{ Recreate a new DN for pql_bind9_update_serial() below.
			$dn_parts = split(',', $dn);
			for($i=1; $dn_parts[$i]; $i++) {
			  $newdn .= $dn_parts[$i];
			  if($dn_parts[$i+1])
				$newdn .= ',';
			}
			$dn = 'relativeDomainName=@,'.$newdn;
			// }}}

			$msg = pql_complete_constant($LANG->_('Successfully removed %value%'),
										 array('value' => $host.'.'.$_REQUEST["dns_domain_name"]));
		  } else 
			$msg = pql_complete_constant($LANG->_('Failed to remove %value%'),
										 array('value' => $host.'.'.$_REQUEST["dns_domain_name"]))
			  . ": " . ldap_error($_pql->ldap_linkid);
		}
		break;
		// }}}
	}

	if($success) {
	  // Successfully modified the zone - update the serial number in the SOA
	  if(!pql_bind9_update_serial($_pql->ldap_linkid, $dn))
		die("failed to update ".$soa["dn"].":".pql_get_define("PQL_ATTR_SOARECORD")." with value $serial_new<br>");  
	}

	if(!file_exists($_SESSION["path"]."/.DEBUG_ME")) 
	  attribute_forward($msg);
	else
	  echo "$msg";
}

// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
