<?php
// attribute plugin for
// BIND9 DNS zone
// $Id: attrib.dnszone.inc,v 2.4 2003-11-20 08:01:37 turbo Exp $

function attribute_check() {
    global $domain, $rootdn, $rdn, $view, $action, $type, $action;
    global $_pql, $PHP_SELF, $attrib, $$attrib, $oldvalue;

    if($attrib && $rdn && ($$attrib != $oldvalue))
      return true;

    return false;
}

function attribute_print_form() {
    global $domain, $rootdn, $rdn, $view, $action, $type, $action, $LANG, $LANG;
    global $_pql, $PHP_SELF, $attrib;

	if($action != 'add') {
		// Get old value
		$dn = urldecode($rdn);
		$oldvalue = pql_bind9_get_record($_pql->ldap_linkid, $dn, $attrib);
		$oldvalue = $oldvalue[0];
	}
?>
    <form action="<?=$PHP_SELF?>" method="post">
      <table cellspacing="0" cellpadding="3" border="0">
        <th colspan="3" align="left"><?=$LANG->_('Change Bind9 value')?></th>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?php echo pql_complete_constant($LANG->_('Change attribute %attribute%'), array('attribute' => $attrib)); ?></td>
            <td><input type="text" name="<?=$attrib?>" value="<?=$oldvalue?>" size="20"></td>
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
            <td class="title"><?=$LANG->_('MX Priority')?>:</td>
            <td><input type="text" name="priority" value="10" size="20"></td>
            <td></td>
          </tr>
<?php	} ?>
        </th>
      </table>

      <input type="hidden" name="submit"   value="1">
<?php	if($action != 'add') { ?>
      <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
<?php	} ?>
      <input type="hidden" name="attrib"   value="<?=$attrib?>">
      <input type="hidden" name="type"     value="<?=$type?>">
      <input type="hidden" name="domain"   value="<?=$domain?>">
      <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
      <input type="hidden" name="view"     value="<?=$view?>">
      <input type="hidden" name="action"   value="<?=$action?>">
      <input type="hidden" name="rdn"      value="<?=$rdn?>">
      <br>
      <input type="submit" value="<?=$LANG->_('Save')?>">
    </form>
<?php
}

function attribute_save($action) {
    global $domain, $rootdn, $rdn, $view, $action, $type, $action, $LANG, $LANG;
    global $_pql, $PHP_SELF, $attrib, $$attrib, $oldvalue, $priority, $time;

	if((lc($attrib) == 'mxrecord') && $priority)
	  $$attrib = $priority . " " . $$attrib;

	if((lc($attrib) == 'dnsttl') && $time) {
	  if($time == "minutes")
		$$attrib = $$attrib * 60;
	  elseif($time == "hours")
		$$attrib = $$attrib * 60 * 60;
	  elseif($time == "days")
		$$attrib = $$attrib * 60 * 60 * 24;
	  elseif($time == "weeks")
		$$attrib = $$attrib * 60 * 60 * 24 * 7;
	  elseif($time == "months")
		$$attrib = $$attrib * 60 * 60 * 24 * 7 * 4;
	}

    switch($action) {
	  case "add":
		// Get old value
		$oldvalues = pql_bind9_get_record($_pql->ldap_linkid, $rdn, $attrib);

		// Add the old values to the array
		for($i=0; $oldvalues[$i]; $i++)
		  $entry[$attrib][] = $oldvalues[$i];

		// Add the new value to the array
		$entry[$attrib][] = $$attrib;

		if(pql_bind9_set_record($_pql->ldap_linkid, $rdn, $entry))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $$attrib)).".";
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $$attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		break;

	  case "modify":
		if(pql_bind9_set_record($_pql->ldap_linkid, $rdn, $attrib, $$attrib))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $attrib));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		break;

	  case "del":
		// Get old value
		$oldvalues = pql_bind9_get_record($_pql->ldap_linkid, $rdn, $attrib);
		for($i=0; $oldvalues[$i]; $i++) {
			// Filter out the value to delete
			if($oldvalues[$i] != $oldvalue)
			  $entry[$attrib][] = $oldvalues[$i];
		}

		if(pql_bind9_set_record($_pql->ldap_linkid, $rdn, $entry))
		  $msg = pql_complete_constant($LANG->_('Successfully removed %value%'),
									   array('value' => $oldvalue));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to remove %value%'),
									   array('value' => $oldvalue)) . ": " . ldap_error($_pql->ldap_linkid);
		break;
	}

    attribute_forward($msg);
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
