<?php
// attribute plugin for
// BIND9 DNS zone
// $Id: attrib.dnszone.inc,v 2.1 2003-06-24 07:43:48 turbo Exp $

function attribute_check() {
    global $domain, $rootdn, $rdn, $view, $config, $action, $type, $action;
    global $_pql, $PHP_SELF, $attrib, $$attrib, $oldvalue;

    if($attrib && $rdn && ($$attrib != $oldvalue))
      return true;

    return false;
}

function attribute_print_form() {
    global $domain, $rootdn, $rdn, $view, $config, $action, $type, $action;
    global $_pql, $PHP_SELF, $attrib;

	if($action != 'add') {
		// Get old value
		$dn = urldecode($rdn);
		$oldvalue = pql_bind9_get_record($_pql->ldap_linkid, $dn, $attrib);
		$oldvalue = $oldvalue[0];
	}
?>
    <form action="<?php echo $PHP_SELF ?>" method="post">
      <table cellspacing="0" cellpadding="3" border="0">
        <th colspan="3" align="left">Change Bind9 value</th>
          <tr class="<?php table_bgcolor(); ?>">
            <td class="title">Change <?=$attrib?></td>
            <td><input type="text" name="<?=$attrib?>" value="<?=$oldvalue?>" size="20"></td>
<?php	if(lc($attrib) == 'dnsttl') { ?>
            <td>
              <select name="time">
                <option value="seconds" SELECTED>Seconds</option>
                <option value="minutes">Minutes</option>
                <option value="hours">Hours</option>
                <option value="days">Days</option>
                <option value="weeks">Weeks</option>
                <option value="months">Months</option>
              </select>
            </td>
<?php	} else { ?>
            <td></td>
<?php	} ?>
          </tr>
<?php	if(lc($attrib) == 'mxrecord') { ?>

          <tr class="<?php table_bgcolor(); ?>">
            <td class="title">MX Priority:</td>
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
      <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
    </form>
<?php
}

function attribute_save($action) {
    global $domain, $rootdn, $rdn, $view, $config, $action, $type, $action;
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
		  $msg = "Successfully added ".$$attrib.".";
		else
		  $msg = "Failed to add ".$$attrib.": " . ldap_error($_pql->ldap_linkid);
		break;

	  case "modify":
		if(pql_bind9_set_record($_pql->ldap_linkid, $rdn, $attrib, $$attrib))
		  $msg = "Successfully replaced $attrib.";
		else
		  $msg = "Failed to replace $attrib: " . ldap_error($_pql->ldap_linkid);
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
		  $msg = "Successfully removed $oldvalue.";
		else
		  $msg = "Failed to remove $oldvalue: " . ldap_error($_pql->ldap_linkid);
		break;
	}

    attribute_forward($msg);
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>


