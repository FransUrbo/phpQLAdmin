<?php
// attribute plugin for
// BIND9 DNS zone
// $Id: attrib.dnszone.inc,v 2.31 2007-09-29 21:15:12 turbo Exp $

// {{{ attribute_translate_type(attrib)
function attribute_translate_type($attrib) {
  // Translate the TYPE to dNSZone objectclass attribute
  switch($attrib) {
	case "a":
	  return pql_get_define("PQL_ATTR_ARECORD");
	case "host":
	  return pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME");
	case "ttl":
	  return pql_get_define("PQL_ATTR_DNSTTL");
	case "ns":
	  return pql_get_define("PQL_ATTR_NSRECORD");
	case "mx":
	  return pql_get_define("PQL_ATTR_MXRECORD");
	case "cname":
	  return pql_get_define("PQL_ATTR_CNAMERECORD");
	case "srv":
	  return pql_get_define("PQL_ATTR_SRVRECORD");
	case "txt":
	  return pql_get_define("PQL_ATTR_TXTRECORD");
	case "afsdb":
	  return pql_get_define("PQL_ATTR_AFSRECORD");

	default:
	  return $_REQUEST["attrib"];
  }
}
// }}}

// {{{ attribute_check(void)
function attribute_check() {
  global $error, $LANG;

  if((lc($attrib) == 'mxrecord') or (lc($attrib) == 'afsdbrecord')) {
	if(empty($_REQUEST["priority"])) {
	  $error["priority"] = $LANG->_('Missing');
	  return false;
	}
  }

  if(empty($_REQUEST["newvalue"])) {
	$error["newvalue"] = $LANG->_('Missing');
	return false;
  }

  if(lc($attrib) == 'dnsttl') {
	if(empty($_REQUEST["dnsttl"])) {
	  $error["dnsttl"] = $LANG->_('Missing');
	  return false;
	}
  }

  return true;
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
	global $_pql, $LANG, $error;

	$type = $_REQUEST["type"];
	$type_value = $_REQUEST[$type];

	if($_REQUEST["attrib"])
	  $attrib = attribute_translate_type($_REQUEST["attrib"]);

	// {{{ Get old value
	if(($_REQUEST["action"] != 'add') and @$_REQUEST["rdn"]) {
		$dn = urldecode($_REQUEST["rdn"]);
		$_REQUEST["oldvalue"] = $_pql->get_attribute($dn, $attrib);
		if(is_array($_REQUEST["oldvalue"])) {
		  for($i=0; $i < count($_REQUEST["oldvalue"]); $i++) {
			if(@preg_match("/$type_value/i", $_REQUEST["oldvalue"][$i]))
			  $oldvalue = $_REQUEST["oldvalue"][$i];
		  }

		  $_REQUEST["oldvalue"] = $oldvalue;
		}
	}
	// }}}

	if((lc($attrib) == 'mxrecord') or (lc($attrib) == 'afsdbrecord')) {
		if($_REQUEST["oldvalue"]) {
			$tmp  = explode(' ', $_REQUEST["oldvalue"]);
			$prio = $tmp[0];
			$_REQUEST["oldvalue"] = $tmp[1];
		} else
		  $prio = '10';
	}

	// relativeDomainName zoneName
	if(@$dn) {
	  $host   = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME"));
	  if(is_array($host))
		$host = $host[0];
	  $domain = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_ZONENAME"));
	  if(is_array($domain))
		$domain = $domain[0];
	  
	  if($host != "@")
		$host = $host.".".$domain;
	}

	if(!isset($error) and (@$_REQUEST["domain"] or @$_REQUEST["rootdn"] or $_REQUEST["rdn"])) {
	  // Only do this the FIRST time!
	  // TODO: This probably have to be done in ALL the attrib.*.inc files!!
	  $_REQUEST["domain"] = urlencode($_REQUEST["domain"]);
	  $_REQUEST["rootdn"] = urlencode($_REQUEST["rootdn"]);
	  $_REQUEST["rdn"]    = urlencode($_REQUEST["rdn"]);
	}
?>
    <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="0" cellpadding="3" border="0">
        <th colspan="3" align="left"><?php
	   if(lc($attrib) == sprintf("%s", pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME"))) {
		 echo pql_complete_constant($LANG->_("Rename DNS host %host%"), array('host' => $host));
	   } elseif($_REQUEST["host"] == 'Global') {
		 echo $LANG->_("Replace value for all entries");
	   } elseif($host != "@") {
		 echo pql_complete_constant($LANG->_('Change Bind9 value for %host%'), array('host' => $host));
	   } else {
		 echo pql_complete_constant($LANG->_("Change Bind9 SOA value for %domain%"), array('domain' => $domain));
	   }
?>
        </th>
<?php	if((lc($attrib) == 'mxrecord') or (lc($attrib) == 'afsdbrecord')) { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?php echo $LANG->_('Priority')?>:</td>
            <td><?php echo pql_format_error_span($error["priority"]); ?><input type="text" name="priority" value="<?php echo $prio?>" size="20"></td>
            <td></td>
          </tr>

<?php	}

		if($_REQUEST["host"] == 'Global') {
?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?php echo $LANG->_('Replace old value')?></td>
            <td><?php echo pql_format_error_span($error["oldvalue"]); ?><input type="text" name="oldvalue" value="<?php echo $_REQUEST["oldvalue"]?>" size="20"></td>
          </tr>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?php echo $LANG->_('With new value')?></td>
            <td><?php echo pql_format_error_span($error["newvalue"]); ?><input type="text" name="newvalue" value="<?php echo $_REQUEST["newvalue"]?>" size="20"></td>
          </tr>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title" align="right"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
            <td>
              <?php echo $LANG->_('Note that this will change \bany\B\nand \ball\B occurences of the IP\nyou specify in \uold value\U with\nthe IP of the value you specify\nin \unew value\U!!')?>
            </td>
          </tr>
<?php	} else { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?php echo pql_complete_constant($LANG->_('Change attribute %attribute%'), array('attribute' => $attrib)); ?></td>
            <td><?php echo pql_format_error_span($error["newvalue"]); ?><input type="text" name="newvalue" value="<?php echo $_REQUEST["oldvalue"]?>" size="20"></td>
<?php	  if(lc($attrib) == 'dnsttl') { ?>
            <td>
			  <?php echo pql_format_error_span($error[""]); ?>
              <select name="time">
                <option value="seconds" SELECTED><?php echo $LANG->_('Seconds')?></option>
                <option value="minutes"><?php echo $LANG->_('Minutes')?></option>
                <option value="hours"><?php echo $LANG->_('Hours')?></option>
                <option value="days"><?php echo $LANG->_('Days')?></option>
                <option value="weeks"><?php echo $LANG->_('Weeks')?></option>
                <option value="months"><?php echo $LANG->_('Months')?></option>
              </select>
            </td>
<?php	  } else { ?>
            <td></td>
<?php	  } ?>
          </tr>
<?php	} ?>

          <input type="hidden" name="old_type" value="<?php echo preg_replace('/record/', '', $attrib)?>">
<?php	if((lc($attrib) == 'mxrecord') or (lc($attrib) == 'afsdbrecord')) { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title" align="right">
              <img src="images/info.png" width="16" height="16" alt="" border="0" align="right">
            </td>
            <td><?php echo $LANG->_("If this is an FQDN not in this zone,<br>don't forget the trailing dot!")?></td>
            <td></td>
          </tr>
<?php	} elseif((lc($attrib) != 'nsrecord') and (lc($attrib) != 'dnsttl') and (lc($attrib) != pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME"))
				 and ($_REQUEST["host"] != 'Global')) {
?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title">New record type</td>
            <td>
              <select name="type">
                <option value="a"<?php     if($attrib == 'arecord')     { echo " SELECTED"; } ?>>A</option>
                <option value="cname"<?php if($attrib == 'cnamerecord') { echo " SELECTED"; } ?>>CNAME</option>
                <option value="hinfo"<?php if($attrib == 'hinforecord') { echo " SELECTED"; } ?>>HINFO</option>
                <option value="mx"<?php    if($attrib == 'mxrecord')    { echo " SELECTED"; } ?>>MX</option>
                <option value="ns"<?php    if($attrib == 'nsrecord')    { echo " SELECTED"; } ?>>NS</option>
                <option value="afsdb"<?php if($attrib == 'afsdbrecord') { echo " SELECTED"; } ?>>AFSDB</option>
              </select>
            </td>
            <td></td>
<?php   } ?>
        </th>
      </table>

      <input type="hidden" name="submit"          value="1">
<?php	if($action != 'add') {
			if((lc($attrib) == 'mxrecord') or (lc($attrib) == 'mxrecord')) {
?>
      <input type="hidden" name="oldvalue"        value="<?php echo $prio." ".$_REQUEST["oldvalue"]?>">
<?php		} elseif($_REQUEST["host"] != 'Global') { ?>
      <input type="hidden" name="oldvalue"        value="<?php echo $_REQUEST["oldvalue"]?>">
<?php		} ?>
<?php	} ?>
      <input type="hidden" name="type"            value="<?php echo $type?>">
      <input type="hidden" name="domain"          value="<?php echo $_REQUEST["domain"]?>">
      <input type="hidden" name="rootdn"          value="<?php echo $_REQUEST["rootdn"]?>">
      <input type="hidden" name="view"            value="<?php echo $_REQUEST["view"]?>">
<?php	if(lc($attrib) == sprintf("%s", pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME"))) { ?>
      <input type="hidden" name="action"          value="rename">
<?php	} else { ?>
      <input type="hidden" name="action"          value="<?php echo $_REQUEST["action"]?>">
<?php	} ?>
      <input type="hidden" name="rdn"             value="<?php echo $_REQUEST["rdn"]?>">
      <input type="hidden" name="dns_domain_name" value="<?php echo $_REQUEST["dns_domain_name"]?>">
<?php	if($_REQUEST["host"] or $_REQUEST["ref"]) { // Called from host_details-dns.inc ?>
      <input type="hidden" name="host"            value="<?php echo $_REQUEST["host"]?>">
      <input type="hidden" name="ref"             value="<?php echo $_REQUEST["ref"]?>">
<?php } ?>
      <br>
      <input type="submit" value="<?php echo $LANG->_('Save')?>">
    </form>
<?php
}
// }}}

// {{{ attribute_save(action)
function attribute_save($action) {
	global $_pql, $LANG;
	$success = 0;
	printr($_REQUEST);

	if($_REQUEST["rdn"])
	  $dn = urldecode($_REQUEST["rdn"]);

	if($_REQUEST["type"]) {
	  $attrib = attribute_translate_type($_REQUEST["type"]);
	  $type = $_REQUEST["type"];
	}

	if($_REQUEST["old_type"])
	  $oldtype = attribute_translate_type($_REQUEST["old_type"]);

	if($_REQUEST["newvalue"]) {
	  $newvalue = $_REQUEST["newvalue"];
	  
	  if((@(lc($attrib) == 'mxrecord') or @(lc($attrib) == 'afsdbrecord')) && isset($_REQUEST["priority"]))
		$newvalue = $_REQUEST["priority"] . " " . $newvalue;
	}

	// {{{ if($attrib == 'dnsttl')
	if((lc($attrib) == 'dnsttl') && $_REQUEST["time"]) {
	  if($_REQUEST["time"] == "minutes")
		$newvalue = $newvalue * 60;
	  elseif($_REQUEST["time"] == "hours")
		$newvalue = $newvalue * 60 * 60;
	  elseif($_REQUEST["time"] == "days")
		$newvalue = $newvalue * 60 * 60 * 24;
	  elseif($_REQUEST["time"] == "weeks")
		$newvalue = $newvalue * 60 * 60 * 24 * 7;
	  elseif($_REQUEST["time"] == "months")
		$newvalue = $newvalue * 60 * 60 * 24 * 7 * 4;
	}
	// }}}

	// {{{ Recreate a new DN for pql_bind9_update_serial() below.
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

    switch($action) {
	  case "add":
		// {{{ add
		// Get old value
		$oldvalues = $_pql->get_attribute($dn, $attrib);
		if($oldvalues and !is_array($oldvalues))
		  $oldvalues = array($oldvalues);

		// Add the old values to the array
		for($i=0; $oldvalues[$i]; $i++)
		  $entry[$attrib][] = $oldvalues[$i];

		// Add the new value to the array
		$entry[$attrib][] = $newvalue;

		if(pql_modify_attribute($dn, '', '', $entry)) {
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
		if($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME"))) {
			// We must also change the RDN!
			$dnattrs = explode(",", urldecode($dn));

			$new_dn  = pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")."=$newvalue";
			for($i=1; $i < count($dnattrs); $i++) {
				$parent .= $dnattrs[$i];
				if($dnattrs[$i+1])
				  $parent .= ",";
			}

			if($_pql->rename($dn, $parent, $new_dn))
			  $dn = $new_dn.','.$parent;
			else
			  die();
		}
		// }}}

	  	// {{{ if(oldtype != newtype)
		if(lc($oldtype) != lc($attrib)) {
		  // Remove the 'oldtype' value
		  if(!pql_modify_attribute($dn, $oldtype, '', ''))
			// This shouldn't really be possible, but until I find out that/if I'm wrong, I'll just die here!
			die("Failed to remove the old attribute '$oldtype' with value ".$_REQUEST["oldvalue"]);
		}
		// }}}

		if((lc($attrib) == 'mxrecord') or (lc($attrib) == 'nsrecord') or ($_REQUEST["host"] == 'Global'))
		  $oldvalue = $_REQUEST["oldvalue"];

		// {{{ Get old value(s) - or change aRecord (etc) globally)
		if(@$dn) {
		  $oldvalues = $_pql->get_attribute($dn, $attrib);
		  if($oldvalues and !is_array($oldvalues))
			$oldvalues = array($oldvalues);
		  
		  for($i=0; $oldvalues[$i]; $i++) {
			if($oldvalues[$i] == $_REQUEST["oldvalue"])
			  $oldvalues[$i] = $newvalue;
		  }
		} elseif($_REQUEST["host"] == 'Global') {
		  // {{{ Change aRecords globaly
		  $CHANGE = array();
		  foreach($_pql->ldap_basedn as $dn)  {
			$dn = pql_format_normalize_dn($dn);
			$filter = "$attrib=$oldvalue";
			$oldvalues = $_pql->search($dn, $filter);
			if(is_array($oldvalues)) {
			  if($oldvalues and is_array($oldvalues) and !$oldvalues[0])
				// only one hit - flat object. Modify it for the for loop below
				$oldvalues = array($oldvalues);

			  for($i=0; $oldvalues[$i]; $i++) {
				$zone_name = $oldvalues[$i]["zonename"];
				if(isset($oldvalues[$i][$attrib])) {
				  // Only one value, and since this object IS in the
				  // array (i.e. returned from search), this should be changed
				  $CHANGE[$zone_name][] = $oldvalues[$i]["dn"];
				}
			  }
			}

			// Now we have a much simpler array, sorted by zoneName and
			// only containing DN's where there's one or more $attrib to
			// change... Go through this array and do the modifications...
			$msg = '';
			foreach($CHANGE as $zone => $dns) {
			  foreach($dns as $dn) {
				$oldvalues = $_pql->get_attribute($dn, $attrib);
				if(is_array($oldvalues)) {
				  // Oups, multiple values! Find the correct one.
				  unset($entry);
				  for($i=0; $oldvalues[$i]; $i++) {
					if($oldvalues[$i] != $oldvalue)
					  $entry[$attrib][] = $oldvalues[$i];
				  }
				  $entry[$attrib][] = $newvalue;
				} else {
				  unset($entry);
				  $entry[$attrib] = $newvalue;
				}

				// NOTE: Why where I using modify() directly here?!?
				//if($_pql->modify($dn, $entry, "attrib.dnszone.inc"))
				if(pql_modify_attribute($dn, '', '', $entry))
				  $success = 1;
				else
				  $msg .= pql_complete_constant($LANG->_("Failed to change %dn%<br>"),
												array('dn' => $dn));
			  }				

			  // This zone is all done. Change it's SOA
			  if(!pql_bind9_update_serial($dn))
				die("failed to update SOA serial for $dn");
			}
		  }
		  // }}}

		  // {{{ Change some other attributes with IP addresses
		  $attribs = array(pql_get_define("PQL_ATTR_IPHOSTNUMBER"),
						   pql_get_define("PQL_ATTR_WEBSRV_SRV_IP"),
						   pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME"));

		  // NOTE: Missing attribute => ApacheAllow ('ApacheAllow: from 1.2.3.4')
		  // NOTE: Setting of success=1 here is broken. If change 1 failed,
		  //       but change 2 succeeded, then we still get success a message!
		  foreach($_pql->ldap_basedn as $dn)  {
			unset($entry);
			$dn = pql_format_normalize_dn($dn);

			for($k=0; $attribs[$k]; $k++) {
			  $filter = $attribs[$k]."=$oldvalue*";
			  $oldvalues = $_pql->get_dn($dn, $filter);
			  for($i=0; $oldvalues[$i]; $i++) {
				// Get old value(s)
				$old = $_pql->get_attribute($oldvalues[$i], $attribs[$k]);
				if(is_array($old)) {
// NOTE: Untested...
//				  for($j=0; $old[$j]; $j++) {
//					if($attribs[$k] == sprintf("%s", pql_get_define("PQL_ATTR_WEBSRV_SRV_IP"))) {
//					  // Split the value to get ip:port
//					  $tmp = explode(':', $old[$j]);
//					  
//					  if($tmp[0] != $oldvalue)
//						// Add old value (in LDAP) != old value (from form)
//						$entry[$attribs[$k]] = $old[$j];
//					} else {
//					  if($old[$j] != $oldvalue)
//						// Add old value (in LDAP) != old value (from form)
//						$entry[$attribs[$k]] = $old[$j];
//					}
//				  }
//				  // Add the new value
//				  if($attribs[$k] == sprintf("%s", pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")))
//					$entry[$attribs[$k]] = $_REQUEST["newvalue"].":".$tmp[1];
//				  else 
//					$entry[$attribs[$k]] = $_REQUEST["newvalue"];
//				  
//				  if(pql_modify_attribute($oldvalues[$i], '', 1, $entry))
//					$success = 1;
				} else {
				  // Split the value to get ip:port
				  if($attribs[$k] == sprintf("%s", pql_get_define("PQL_ATTR_WEBSRV_SRV_IP"))) {
					$tmp = explode(':', $old);
					$new = $_REQUEST["newvalue"].":".$tmp[1];
				  } elseif($attribs[$k] == sprintf("%s", pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME"))) {
					// Objects have this as DN. Do a modrdn!
					$dnattrs = explode(",", $oldvalues[$i]); unset($parent);
					$new_rdn = pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")."=".$_REQUEST["newvalue"];

					// Put togheter a new DN
					for($z=1; $z < count($dnattrs); $z++) {
					  $parent .= $dnattrs[$z];
					  if($dnattrs[$z+1])
						$parent .= ",";
					}

					if($_pql->rename($oldvalues[$i], $parent, $new_rdn))
					  $success = 1;
				  } else
					$new = $_REQUEST["newvalue"];

				  if($attribs[$k] != pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")) {
					if(pql_modify_attribute($oldvalues[$i], $attribs[$k], 1, $new))
					  $success = 1;
				  }
				}
			  }
			}
		  }
		  // }}}

		  if($success)
			$msg .= pql_complete_constant($LANG->_("Successfully replaced %old% with %new%"),
										  array('old' => $oldvalue, 'new' => $newvalue));

		  // Well, we're actually done now!
		  attribute_forward($msg);
		}
		// }}}

		if(($attrib == 'dnsttl') and preg_match('/^dnsttl/i', $dn)) {
		  // The DN is encoded with the dNSTTL value, so that must be changed as well!
		  // dNSTTL=3600+relativeDomainName=@
		  // =>
		  // dNSTTL=2419200+relativeDomainName=@
		  $dn_parts = explode(',', $dn);

		  // {{{ Put togehter a parent DN
		  $parent = '';
		  for($i=1; $dn_parts[$i]; $i++) {
			$parent .= $dn_parts[$i];
			if($dn_parts[$i+1])
			  $parent .= ',';
		  }
		  // }}}

		  // Change old to a new value
		  $new = "dNSTTL=".$oldvalues[0]."+relativeDomainName=@";

		  // Do the rename...
		  echo "rename('".$dn_parts[0]."', '$parent', '$new', 'FALSE')<br>";
		  if($_pql->rename($dn_parts[0], $parent, $new, FALSE)) {
			$success = 1;

			// Put togheter a new SOA DN for SOA serial update below
			$dn_soa = $new.",".$parent;
		  } else
			die();
		} else {
		  if(pql_modify_attribute($dn, $attrib, '', $oldvalues)) {
			$success = 1;
			$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										 array('what' => $attrib));
		  } else
			$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										 array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		}
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

		if($_pql->rename($rdn_old, $parent, $rdn_new))
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
		if($_REQUEST["type"] and ($_REQUEST["oldvalue"] or $_REQUEST[$type])) {
			// Get old value
			$oldvalues = $_pql->get_attribute($dn, $attrib);
			if($oldvalues and !is_array($oldvalues))
			  $oldvalues = array($oldvalues);

			for($i=0; $oldvalues[$i]; $i++) {
			  // Filter out the value to delete
			  if($_REQUEST["oldvalue"]) {
				if($oldvalues[$i] != $_REQUEST["oldvalue"])
				  $entry[$attrib][] = $oldvalues[$i];
			  } elseif($_REQUEST[$type]) {
				if($oldvalues[$i] != $_REQUEST[$type])
				  $entry[$attrib][] = $oldvalues[$i];
			  }
			}

			if(!is_array($entry)) {
			  // {{{ This is/was the last value for this attribute
			  //     but there might be more attributes in the object.
			  //     If so, we can NOT remove the object!!!

			  // Get the whole object.
			  $object = $_pql->search($dn, 'objectClass=*', 'BASE');

			  // Remove the attribute we where asked to remove
			  unset($object[$attrib]);

			  // Remove the most obvious attributes from the object
			  // (i.e. attributes that shouldn't hold of a object delete)
			  // TODO: This must be better tested.
			  unset($object[pql_get_define("PQL_ATTR_OBJECTCLASS")]);
			  unset($object[pql_get_define("PQL_ATTR_DNSTTL")]);
			  unset($object[pql_get_define("PQL_ATTR_ZONENAME")]);
			  unset($object[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")]);
			  unset($object["dn"]);

			  // If there's anything left, we must MODIFY the object, not DELETE it.
			  // Usually this is any aRecord value(s)...
			  if(is_array($object) and count($object))
				$entry[$attrib] = array();
			  // }}}
			}

			if(is_array($entry)) {
			  if(pql_modify_attribute($dn, '', '', $entry)) {
				$success = 1;
				$msg = pql_complete_constant($LANG->_('Successfully removed %value%'),
											 array('value' => $_REQUEST["oldvalue"]));
			  } else
				$msg = pql_complete_constant($LANG->_('Failed to remove %value%'),
											 array('value' => $_REQUEST["oldvalue"])) . ": " . ldap_error($_pql->ldap_linkid);
			} else {
			  // No more oldvalues - remove the whole object

			  // Extact the host part of the DN
			  $host = explode(',', $dn); $host = explode('=', $host[0]); $host = $host[1];

			  if($_pql->delete($dn)) {
				$success = 1;
				$msg = pql_complete_constant($LANG->_('Successfully removed %value%'),
											 array('value' => $host.'.'.$_REQUEST["dns_domain_name"]));
			  } else
				$msg = pql_complete_constant($LANG->_('Failed to remove %value%'),
											 array('value' => $host.'.'.$_REQUEST["dns_domain_name"]))
				  . ": " . ldap_error($_pql->ldap_linkid);
			}
		} else {
		  // Delete a whole object
		  if($_pql->delete($dn))
			$success = 1;
		  else
			pql_format_error(1);

		  // Extact the host part of the DN
		  $host = explode(',', $dn); $host = explode('=', $host[0]); $host = $host[1];

		  if($success) {
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
	  if(!pql_bind9_update_serial($dn_soa))
		die("failed to update ".$soa["dn"].":".pql_get_define("PQL_ATTR_SOARECORD")." with value $serial_new<br>");  
	}

	attribute_forward($msg);
}

// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
