<?php
// attribute plugin for
// BIND9 DNS zone
// $Id: attrib.dnszone.inc,v 2.5.12.1 2004-12-19 15:50:52 turbo Exp $

// {{{ attribute_check(void)
function attribute_check() {
	$attrib = $_REQUEST[$attrib];

    if($_REQUEST["attrib"] && $_REQUEST["rdn"] && ($attrib != $_REQUEST["oldvalue"]))
      return true;

    return false;
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $_pql, $LANG;

	if($_REQUEST["action"] != 'add') {
		// Get old value
		$dn = urldecode($_REQUEST["rdn"]);
		$oldvalue = pql_bind9_get_record($_pql->ldap_linkid, $dn, $_REQUEST["attrib"]);
		$oldvalue = $oldvalue[0];
	}
?>
    <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="0" cellpadding="3" border="0">
        <th colspan="3" align="left"><?=$LANG->_('Change Bind9 value')?></th>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?php echo pql_complete_constant($LANG->_('Change attribute %attribute%'), array('attribute' => $_REQUEST["attrib"])); ?></td>
            <td><input type="text" name="<?=$_REQUEST["attrib"]?>" value="<?=$oldvalue?>" size="20"></td>
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
            <td class="title"><?=$LANG->_('MX Priority')?>:</td>
            <td><input type="text" name="priority" value="10" size="20"></td>
            <td></td>
          </tr>
<?php	} ?>
        </th>
      </table>

      <input type="hidden" name="submit"   value="1">
<?php	if($_REQUEST["action"] != 'add') { ?>
      <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
<?php	} ?>
      <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
      <input type="hidden" name="type"     value="<?=$_REQUEST["type"]?>">
      <input type="hidden" name="domain"   value="<?=$_REQUEST["domain"]?>">
      <input type="hidden" name="rootdn"   value="<?=$_REQUEST["rootdn"]?>">
      <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
      <input type="hidden" name="action"   value="<?=$_REQUEST["action"]?>">
      <input type="hidden" name="rdn"      value="<?=$_REQUEST["rdn"]?>">
      <br>
      <input type="submit" value="<?=$LANG->_('Save')?>">
    </form>
<?php
}
// }}}

// {{{ attribute_save(action)
function attribute_save($action) {
    global $_pql, $LANG;
	$attrib = $_REQUEST["attrib"];

	if((lc($attrib) == 'mxrecord') && $_REQUEST["priority"])
	  $_REQUEST[$attrib] = $_REQUEST["priority"] . " " . $_REQUEST[$attrib];

	if((lc($attrib) == 'dnsttl') && $_REQUEST["time"]) {
	  if($_REQUEST["time"] == "minutes")
		$_REQUEST[$attrib] = $_REQUEST[$attrib] * 60;
	  elseif($_REQUEST["time"] == "hours")
		$_REQUEST[$attrib] = $_REQUEST[$attrib] * 60 * 60;
	  elseif($_REQUEST["time"] == "days")
		$_REQUEST[$attrib] = $_REQUEST[$attrib] * 60 * 60 * 24;
	  elseif($_REQUEST["time"] == "weeks")
		$_REQUEST[$attrib] = $_REQUEST[$attrib] * 60 * 60 * 24 * 7;
	  elseif($_REQUEST["time"] == "months")
		$_REQUEST[$attrib] = $_REQUEST[$attrib] * 60 * 60 * 24 * 7 * 4;
	}

    switch($action) {
	  case "add":
		// Get old value
		$oldvalues = pql_bind9_get_record($_pql->ldap_linkid, $_REQUEST["rdn"], $attrib);

		// Add the old values to the array
		for($i=0; $oldvalues[$i]; $i++)
		  $entry[$attrib][] = $oldvalues[$i];

		// Add the new value to the array
		$entry[$attrib][] = $_REQUEST[$attrib];

		if(pql_bind9_set_record($_pql->ldap_linkid, $rdn, $entry))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $_REQUEST[$attrib])).".";
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $_REQUEST[$attrib])) . ": " . ldap_error($_pql->ldap_linkid);
		break;

	  case "modify":
		if(pql_bind9_set_record($_pql->ldap_linkid, $rdn, $attrib, $_REQUEST[$attrib]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $attrib));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		break;

	  case "del":
		if($_REQUEST["type"] and $_REQUEST["oldvalue"]) {
			// Get old value
			$oldvalues = pql_bind9_get_record($_pql->ldap_linkid, $rdn, $attrib);
			for($i=0; $oldvalues[$i]; $i++) {
				// Filter out the value to delete
				if($oldvalues[$i] != $oldvalue)
				  $entry[$attrib][] = $oldvalues[$i];
			}
			
			if(pql_bind9_set_record($_pql->ldap_linkid, $_REQUEST["rdn"], $entry))
			  $msg = pql_complete_constant($LANG->_('Successfully removed %value%'),
										   array('value' => $oldvalue));
			else
			  $msg = pql_complete_constant($LANG->_('Failed to remove %value%'),
										   array('value' => $oldvalue)) . ": " . ldap_error($_pql->ldap_linkid);
		} else {
			if(pql_bind9_del_host($_pql->ldap_linkid, $_REQUEST["rdn"]))
			  $msg = pql_complete_constant($LANG->_('Successfully removed host %host%'),
										   array('host' => $_REQUEST["rdn"]));
			else
			  $msg = pql_complete_constant($LANG->_('Failed to remove host %host%'),
										   array('value' => $_REQUEST["rdn"])) . ": " . ldap_error($_pql->ldap_linkid);
		}
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
