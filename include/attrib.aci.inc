<?php
// attribute plugin for OpenLDAPaci
// $Id: attrib.aci.inc,v 1.1 2003-11-18 12:44:25 turbo Exp $

function attribute_check() {
    global $attrib, $domain, $oldvalue, $_pql, $error, $$attrib, $type, $LANG;
    global $aci_oid, $aci_scope, $aci_action, $aci_permission_read, $aci_permission_write;
    global $aci_permission_search, $aci_permission_compare, $aci_target, $aci_type, $aci_subject;

    // is typed in ?
    if(!$aci_oid and !$aci_scope and !$aci_action) {
	$error[$attrib] = $LANG->_('Missing');
	return false;
    }

    if($$attrib) {
	$value = $$attrib;
    } else {
	$$attrib  = "$aci_oid#$aci_scope#$aci_action;";

	if($aci_permission_read) {
	    $perm = "r";
	}

	if($aci_permission_write) {
	    if($perm) $perm .= ",";
	    $perm .= "w";
	}

	if($aci_permission_search) {
	    if($perm) $perm .= ",";
	    $perm .= "s";
	}

	if($aci_permission_compare) {
	    if($perm) $perm .= ",";
	    $perm .= "c";
	}

	if(($aci_target == 'all') or ($aci_target == 'children') or ($aci_target == 'entry'))
	  $aci_target = "[" . $aci_target . "]";

	$$attrib .= "$perm;$aci_target#$aci_type#$aci_subject";
    }

    return true;
}

function attribute_print_form($action) {
    global $attrib, $domain, $rootdn, $PHP_SELF, $$attrib, $oldvalue, $error, $_pql, $user, $view, $LANG;
    global $orgname;

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Access Control Information')?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Object DN')?></td>
          <td><?php if($user) { echo "$user"; } else { echo "$domain"; } ?></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('OID')?></td>
          <td><b>1.2.3</b></td>
        </tr>
        <input type="hidden" name="aci_oid" value="1.2.3">

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Scope')?></td>
          <td><b>entry</b></td>
        </tr>
        <input type="hidden" name="aci_scope" value="entry">

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Action')?></td>
          <td>
            <select name="aci_action">
              <option value="grant" SELECTED>Grant</option>
              <option value="deny">Deny</option>
            </select>
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Permissions')?></td>
          <td>
            <input type="checkbox" name="aci_permission_read"><u>r</u>ead
            <input type="checkbox" name="aci_permission_write"><u>w</u>rite
            <input type="checkbox" name="aci_permission_search"><u>s</u>earch
            <input type="checkbox" name="aci_permission_compare"><u>c</u>ompare
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Target')?></td>
          <td>
            <select name="aci_target">
              <option value="all">All attributes</option>
              <option value="entry">Entry itself</option>
              <option value="children">Sub-objects</option>
            </select>
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Type')?></td>
          <td>
            <select name="aci_type">
              <option value="access-id">Specific DN</option>
              <option value="group">Group</option>
              <option value="self">Self</option>
            </select>
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Subject')?></td>
          <td><input type="text" name="aci_subject" size="30"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="rootdn"        value="<?=$rootdn?>">
    <input type="hidden" name="domain"        value="<?=$domain?>">
    <input type="hidden" name="user"          value="<?=$user?>">
    <input type="hidden" name="attrib"        value="<?=$attrib?>">
    <input type="hidden" name="view"          value="<?=$view?>">
    <input type="hidden" name="type"          value="<?=$action?>">
    <input type="hidden" name="submit"        value="1">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function attribute_save($action) {
    global $domain, $rootdn, $attrib, $oldvalue, $$attrib, $_pql, $user, $view, $LANG;
    $value = $$attrib;

    $rootdn = urldecode($rootdn);
    $domain = urldecode($domain);
    $user   = urldecode($user);

    echo "rootdn: $rootdn<br>";
    echo "domain: $domain<br>";
    echo "user: $user<br><br>";

    echo "action: $action<br>";
    echo "attrib: $attrib<br>";
    echo "value: $value<br>";

    // Retreive the old values
    if($user)
      $acis = pql_aci_get($_pql->ldap_linkid, $user);
    else 
      $acis = pql_aci_get($_pql->ldap_linkid, $domain);

    // Extract only the ACI line that's stored in LDAP
    for($i=0; $acis[$i]["LINE"]; $i++) {
	$ACIs[$attrib][] = $acis[$i]["LINE"];
    }
    $acis = $ACIs;

// ------------------
// TODO: Test if pql_replace_attribute() works!
// ------------------

    $success = 0;
    switch($action) {
      case "add":
	// Do the modification
	if($user) {
	    if(pql_replace_attribute($_pql->ldap_linkid, $user, '', $acis))
	      $success = 1;
//	} else {
// TODO: Modify domain object
	}
	break;

      case "modify":
	break;

      case "delete":
	unset($ACIs);

	// Remove specific entry
	for($i=0; $acis[pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")][$i]; $i++) {
	    if($acis[pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")][$i] != $value)
	      // This is not a value to be delete it, remember it
	      $ACIs[pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")][] = $acis[pql_get_define("PQL_GLOB_ATTR_OPENLDAPACI")][$i];
	}

	if($user) {
	    if(pql_replace_attribute($_pql->ldap_linkid, $user, '', $ACIs))
	      $success = 1;
//	} else {
// TODO: Modify domain object
	}
	break;
      default:
	die(pql_complete_constant($LANG->_('Unknown action %action% in file %file%, function save()'),
				  array('action' => $action, 'file' => __FILE__)));
    }

    if($success)
      $msg = pql_complete_constant($LANG->_('Successfully %action% ACI to DN'), array('action' => $action));
    else
      $msg = pql_complete_constant($LANG->_('Failed to %action% ACI to DN'), array('action' => $action))
	. ":&nbsp;" . ldap_error($_pql->ldap_linkid);
    attribute_forward($msg);
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
