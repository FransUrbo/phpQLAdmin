<?php
// attribute plugin for
// BIND9 DNS zone
// $Id: attrib.dnszone.inc,v 2.9 2004-11-05 13:04:40 turbo Exp $

// {{{ attribute_check(void)
function attribute_check() {
	// dummy function
	return true;
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
	global $_pql, $LANG;

	if($_REQUEST["action"] != 'add') {
		// Get old value
		$dn = urldecode($_REQUEST["rdn"]);
		$_REQUEST["oldvalue"] = pql_get_attribute($_pql->ldap_linkid, $dn, $_REQUEST["attrib"]);
		if(is_array($_REQUEST["oldvalue"]))
		  $_REQUEST["oldvalue"] = $_REQUEST["oldvalue"][0];
	}

	if((lc($_REQUEST["attrib"]) == 'mxrecord')) {
		if($_REQUEST["oldvalue"]) {
			$tmp  = split(' ', $_REQUEST["oldvalue"]);
			$prio = $tmp[0];
			$_REQUEST["oldvalue"] = $tmp[1];
		} else
		  $prio = '10';
	}
?>
    <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="0" cellpadding="3" border="0">
        <th colspan="3" align="left"><?=$LANG->_('Change Bind9 value')?></th>
<?php	if(lc($_REQUEST["attrib"]) == 'mxrecord') { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_('MX Priority')?>:</td>
            <td><input type="text" name="priority" value="<?=$prio?>" size="20"></td>
            <td></td>
          </tr>

<?php	} ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?php echo pql_complete_constant($LANG->_('Change attribute %attribute%'), array('attribute' => $_REQUEST["attrib"])); ?></td>
            <td><input type="text" name="<?=$_REQUEST["attrib"]?>" value="<?=$_REQUEST["oldvalue"]?>" size="20"></td>
<?php	if(lc($_REQUEST["attrib"]) == 'dnsttl') { ?>
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
<?php	if(lc($_REQUEST["attrib"]) == 'mxrecord') { ?>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title" align="right">
              <img src="images/info.png" width="16" height="16" alt="" border="0" align="right">
            </td>
            <td><?=$LANG->_("If this is an FQDN not in this zone,<br>don't forget the trailing dot!")?></td>
            <td></td>
          </tr>
<?php   } ?>
        </th>
      </table>

      <input type="hidden" name="submit"   value="1">
<?php	if($action != 'add') { ?>
      <input type="hidden" name="oldvalue" value="<?=$_REQUEST["oldvalue"]?>">
<?php	} ?>
      <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
      <input type="hidden" name="type"     value="<?=$_REQUEST["type"]?>">
      <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
      <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
      <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
      <input type="hidden" name="action"   value="<?=$_REQUEST["action"]?>">
      <input type="hidden" name="rdn"      value="<?=urlencode($_REQUEST["rdn"])?>">
      <br>
      <input type="submit" value="<?=$LANG->_('Save')?>">
    </form>
<?php
}
// }}}

// {{{ attribute_save(action)
function attribute_save($action) {
	global $_pql, $LANG;

	$dn = urldecode($_REQUEST["rdn"]);
	$attrib = $_REQUEST["attrib"];
	$value  = $_REQUEST[$attrib];

	if((lc($attrib) == 'mxrecord') && $_REQUEST["priority"])
	  $value = $_REQUEST["priority"] . " " . $value;

	if((lc($attrib) == 'dnsttl') && $_REQUEST["time"]) {
	  if($_REQUEST["time"] == "minutes")
		$value = $value * 60;
	  elseif($_REQUEST["time"] == "hours")
		$value = $value * 60 * 60;
	  elseif($_REQUEST["time"] == "days")
		$value = $value * 60 * 60 * 24;
	  elseif($_REQUEST["time"] == "weeks")
		$value = $value * 60 * 60 * 24 * 7;
	  elseif($_REQUEST["time"] == "months")
		$value = $value * 60 * 60 * 24 * 7 * 4;
	}

    switch($action) {
	  case "add":
		// Get old value
		$oldvalues = pql_get_attribute($_pql->ldap_linkid, $dn, $attrib);
		if($oldvalues and !is_array($oldvalues))
		  $oldvalues = array($oldvalues);

		// Add the old values to the array
		for($i=0; $oldvalues[$i]; $i++)
		  $entry[$attrib][] = $oldvalues[$i];

		// Add the new value to the array
		$entry[$attrib][] = $value;

		if(pql_modify_attribute($_pql->ldap_linkid, $dn, '', '', $entry))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $value)).".";
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $value)) . ": " . ldap_error($_pql->ldap_linkid);
		break;

	  case "modify":
		if($attrib == pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")) {
			// We must also change the RDN!
			$dnattrs = explode(",", urldecode($dn));

			$new_dn  = pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")."=$value";
			for($i=1; $dnattrs[$i]; $i++) {
				$parent .= $dnattrs[$i];
				if($dnattrs[$i+1])
				  $parent .= ",";
			}

			if(file_exists("./.DEBUG_ME")) {
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


		if(pql_modify_attribute($_pql->ldap_linkid, $dn, $attrib, 1, $value))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $attrib));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		break;

	  case "del":
		// Get old value
		$oldvalues = pql_get_attribute($_pql->ldap_linkid, $dn, $attrib);
		if($oldvalues and !is_array($oldvalues))
		  $oldvalues = array($oldvalues);

		for($i=0; $oldvalues[$i]; $i++) {
			// Filter out the value to delete
			if($oldvalues[$i] != $_REQUEST["oldvalue"])
			  $entry[$attrib][] = $oldvalues[$i];
		}

		if(pql_modify_attribute($_pql->ldap_linkid, $dn, '', '', $entry))
		  $msg = pql_complete_constant($LANG->_('Successfully removed %value%'),
									   array('value' => $_REQUEST["oldvalue"]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to remove %value%'),
									   array('value' => $_REQUEST["oldvalue"])) . ": " . ldap_error($_pql->ldap_linkid);
		break;
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
