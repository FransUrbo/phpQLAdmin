<?php
// attribute plugin for the Webserver Configuration editor
//
// $Id: attrib.websrv.inc,v 1.4 2004-02-14 14:25:36 turbo Exp $

function attribute_check() {
    global $rootdn, $domain, $view, $action, $attrib, $$attrib, $server, $oldvalue;
    global $_pql, $LANG;

    if($attrib && ($$attrib != $oldvalue))
      return true;

    return false;
}

function attribute_print_form() {
    global $rootdn, $domain, $view, $action, $attrib, $server, $oldvalue;
	global $_pql, $LANG;
?>
    <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="0" cellpadding="3" border="0">
        <th colspan="3" align="left"><?=$LANG->_('Change webserver configuration')?></th>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_('Server configuration DN')?>:</td>
            <td><?=$server?></td>
          </tr>

<?php if($oldvalue) { ?>
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_('Old value')?>:</td>
            <td><?=$oldvalue?></td>
          </tr>

<?php } ?>
          <tr class="<?php pql_format_table(); ?>">
<?php if($oldvalue) { ?>
            <td class="title"><?php echo pql_complete_constant($LANG->_('Change attribute %attribute%'), array('attribute' => $attrib)); ?></td>
<?php } else { ?>
            <td class="title"><?php echo pql_complete_constant($LANG->_('Set attribute %attribute%'), array('attribute' => $attrib)); ?></td>
<?php } ?>
            <td><input type="text" name="<?=$attrib?>" value="<?=$$attrib?>" size="20"></td>
          </tr>
        </th>
      </table>

      <input type="hidden" name="submit"   value="1">
      <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
      <input type="hidden" name="domain"   value="<?=$domain?>">
      <input type="hidden" name="server"   value="<?=$server?>">
      <input type="hidden" name="attrib"   value="<?=$attrib?>">
      <input type="hidden" name="view"     value="<?=$view?>">
      <input type="hidden" name="action"   value="<?=$action?>">
      <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
      <br>
      <input type="submit" value="<?=$LANG->_('Save')?>">
    </form>
<?php
}

function attribute_save($action) {
    global $rootdn, $domain, $view, $action, $attrib, $$attrib, $server, $oldvalue;
    global $_pql, $LANG;

    switch($action) {
	  case "add":
		// Fetch the old values from the DB
		$filter = "(&(objectclass=phpQLAdminWebsrv)($attrib=*))";
		$sr   = ldap_read($_pql->ldap_linkid, $server, $filter, array($attrib));
		$info = ldap_get_entries($_pql->ldap_linkid, $sr)
		  or pql_format_error(0);
		for($i=0; $i < $info[0][$attrib]["count"]; $i++) {
			// Add the old value(s) to the array
			$entry[$attrib][] = $info[0][$attrib][$i];
		}

		// Add the new value to the array
		$entry[$attrib][] = $$attrib;

		if(pql_websrv_set_record($_pql->ldap_linkid, $server, $entry))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $$attrib)).".";
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $$attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		break;

	  case "mod":
		if(pql_websrv_set_record($_pql->ldap_linkid, $server, $attrib, $$attrib))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $attrib));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
		break;

	  case "del":
		$entry[$attrib] = array();

		if(ldap_mod_del($_pql->ldap_linkid, $server, $entry))
		  $msg = pql_complete_constant($LANG->_('Successfully removed %value%'),
									   array('value' => $attrib));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to remove %value%'),
									   array('value' => $attrib)) . ": " . ldap_error($_pql->ldap_linkid);
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
