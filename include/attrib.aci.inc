<?php
// attribute plugin for OpenLDAPaci
// $Id: attrib.aci.inc,v 1.11 2005-09-16 06:08:46 turbo Exp $

/* DLW: This has valid syntax, but I don't know how to test it. */

// {{{ attribute_check(void)
function attribute_check() {
    global $_pql, $error, $LANG;
	
	$attrib = $_REQUEST["attrib"];

    // is typed in ?
    if(empty($_REQUEST["aci_oid"]) and empty($_REQUEST["aci_scope"]) and empty($_REQUEST["aci_action"])) {
		$error[$attrib] = $LANG->_('Missing');
		return false;
    }
	
    if($_REQUEST[$attrib]) {
		$value = $_REQUEST[$attrib]; // DLW: Does this do anything?
    } else {
		// Get the next OID number by retreive the old ACI's.
		if($_REQUEST["user"])
		  $acis = pql_aci_get($_pql->ldap_linkid, $_REQUEST["user"]);
		else 
		  $acis = pql_aci_get($_pql->ldap_linkid, $_REQUEST["domain"]);
		$next_oid = $acis['count'];

		$_REQUEST[$attrib] = "$next_oid#" . $_REQUEST["aci_scope"] . '#' . $_REQUEST["aci_action"] . ";";

		$perm = '';
		if($_REQUEST["aci_permission_read"]) {
			$perm = "r";
		}
		
		if($_REQUEST["aci_permission_write"]) {
			if($perm) $perm .= ",";
			$perm .= "w";
		}
		
		if($_REQUEST["aci_permission_search"]) {
			if($perm) $perm .= ",";
			$perm .= "s";
		}
		
		if($_REQUEST["aci_permission_compare"]) {
			if($perm) $perm .= ",";
			$perm .= "c";
		}
		
		if(($_REQUEST["aci_target"] == 'all') or ($_REQUEST["aci_target"] == 'children') or ($_REQUEST["aci_target"] == 'entry'))
		  $_REQUEST["aci_target"] = "[" . $_REQUEST["aci_target"] . "]";
		
		$_REQUEST[$attrib] .= "$perm;" . $_REQUEST["aci_target"] . '#' . $_REQUEST["aci_type"] . '#' . $_REQUEST["aci_subject"];
    }
	
    return true;
}
// }}}

// {{{ attribute_print_form(action)
function attribute_print_form($action) {
    global $_pql, $LANG;

	if(!$_REQUEST["user"])
	  // It must be a domain
	  $aci = pql_aci_get($_pql->ldap_linkid, $_REQUEST["domain"]);
	else
	  // It must be a user
	  $aci = pql_aci_get($_pql->ldap_linkid, $_REQUEST["user"]);

	if(is_array($aci)) {
		echo "<u>Current values:</u><p>";
		for($i=0; $i < count($aci); $i++)
		  echo $aci[$i]["LINE"]."<br>";
	}
?>
  <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Access Control Information')?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Object DN')?></td>
          <td><?php if(!empty($_REQUEST["user"])) { echo $_REQUEST["user"]; } else { echo $_REQUEST["domain"]; } ?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('OID')?></td>
          <td><b>1.2.3</b></td>
        </tr>
        <input type="hidden" name="aci_oid" value="1.2.3">

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Scope')?></td>
          <td><b>entry</b></td>
        </tr>
        <input type="hidden" name="aci_scope" value="entry">

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Action')?></td>
          <td>
            <select name="aci_action">
              <option value="grant" SELECTED>Grant</option>
              <option value="deny">Deny</option>
            </select>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Permissions')?></td>
          <td>
            <input type="checkbox" name="aci_permission_read"><u>r</u>ead
            <input type="checkbox" name="aci_permission_write"><u>w</u>rite
            <input type="checkbox" name="aci_permission_search"><u>s</u>earch
            <input type="checkbox" name="aci_permission_compare"><u>c</u>ompare
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Target')?></td>
          <td>
            <select name="aci_target">
              <option value="all">All attributes</option>
              <option value="entry">Entry itself</option>
              <option value="children">Sub-objects</option>
            </select>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Type')?></td>
          <td>
            <select name="aci_type">
              <option value="access-id">Specific DN</option>
              <option value="group">Group</option>
              <option value="self">Self</option>
            </select>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Subject')?></td>
          <td><input type="text" name="aci_subject" size="30"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title" align="right"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
          <td><?=$LANG->_("The OID value isn't used. This new ACI will be added at the<br>end of the existing list. You can move it yourself later.")?></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="rootdn"        value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="domain"        value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="user"          value="<?=urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="attrib"        value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="view"          value="<?=$_REQUEST["view"]?>">
    <input type="hidden" name="type"          value="<?=$action?>">
    <input type="hidden" name="submit"        value="1">
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
    $value = $_REQUEST[$attrib];

    $rootdn = urldecode($_REQUEST["rootdn"]);
    $domain = urldecode($_REQUEST["domain"]);
    $user   = urldecode($_REQUEST["user"]);

    // Retreive the old values
    if($user)
      $acis = pql_aci_get($_pql->ldap_linkid, $user);
    else 
      $acis = pql_aci_get($_pql->ldap_linkid, $domain);
	
    // Extract only the ACI line that's stored in LDAP
    for($i=0; @$acis[$i]["LINE"]; $i++)
	  $ACIs[$attrib][] = $acis[$i]["LINE"];

	$oidnr = $acis[$i-1]['OID'] + 1;
    $acis  = $ACIs;

    switch($action) {
		// {{{ case: add
      case "add":
		// Add the value to the attribute
		$acis[$attrib][] = $value;
		break;
		// }}}
		
		// {{{ case: modify - TODO
      case "modify":
		break;
		// }}}
		
		// {{{ case: delete
      case "delete":
		unset($ACIs);
		
		// Remove specific entry
		for($i=0; $i < count($acis[pql_get_define("PQL_ATTR_LDAPACI")]); $i++) {
			if($acis[pql_get_define("PQL_ATTR_LDAPACI")][$i] != $value)
			  // This is not a value to be delete it, remember it
			  $ACIs[pql_get_define("PQL_ATTR_LDAPACI")][] = $acis[pql_get_define("PQL_ATTR_LDAPACI")][$i];
		}
		
		$acis = $ACIs;
		break;
		// }}}

		// {{{ case: moveup
	  case "moveup":
		for($i=0; $i < count($acis[$attrib]); $i++) {
			// If the NEXT value is what we're looking for,
			// it will have to be put in THIS position
			if($value == $acis[$attrib][$i+1]) {
				// Next ACI is the one we're looking for - put that here
				$new[$attrib][] = $acis[$attrib][$i+1];

				// Add the current one AFTER this
				$new[$attrib][] = $acis[$attrib][$i];
				$i++;
			} else
			  $new[$attrib][] = $acis[$attrib][$i];
		}

		$acis = $new;
		break;
		// }}}

		// {{{ case: movedown
	  case "movedown":
		for($i=0; $i < count($acis[$attrib]); $i++) {
			if($value == $acis[$attrib][$i]) {
				// This value is the one we're looking for - take next and put that here
				$new[$attrib][] = $acis[$attrib][$i+1];

				// Add the current one AFTER this
				$new[$attrib][] = $acis[$attrib][$i];
				$i++;
			} else
			  $new[$attrib][] = $acis[$attrib][$i];
		}

		$acis = $new;
		break;
		// }}}

		// {{{ case: default
      default:
		die(pql_complete_constant($LANG->_('Unknown action %action% in file %file%, function save()'),
								  array('action' => $action, 'file' => __FILE__)));
		// }}}
    }
	
	// {{{ Do the modification
    $success = 0;
	if($user) {
		if(pql_modify_attribute($_pql->ldap_linkid, $user, '', '', $acis))
		  $success = 1;
	} else {
		if(pql_modify_attribute($_pql->ldap_linkid, $domain, '', '', $acis))
		  $success = 1;
	}
	// }}}

	// {{{ Redirect depending on success 
    if($success)
      $msg = pql_complete_constant($LANG->_('Successfully %action% ACI to DN'), array('action' => $action));
    else
      $msg = pql_complete_constant($LANG->_('Failed to %action% ACI to DN'), array('action' => $action))
		. ":&nbsp;" . ldap_error($_pql->ldap_linkid);
    attribute_forward($msg);
	// }}}
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
