<?php
// attribute for phpQLAdmin/Physical host IP address
// $Id: attrib.hostdhcp.inc,v 2.1 2007-07-19 10:28:01 turbo Exp $

// {{{ attribute_check(void)
function attribute_check() {
  global $error, $LANG;

  $attrib = $_REQUEST["attrib"];
 
  // is typed in ?
  if(empty($_REQUEST[$attrib])) {
	$error[$attrib] = $LANG->_('Missing');
	return false;
  }

  // Not equal to old value
  if($_REQUEST[$attrib] == @$_REQUEST["oldvalue"]) {
	$error[$attrib] = $LANG->_('Same as old value');
	return false;
  }

  return true;
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
  global $LANG, $error;

  $attrib = $_REQUEST["attrib"];
  if($_REQUEST[$attrib])
    $_REQUEST["oldvalue"] = $_REQUEST[$attrib];
  else
	$_REQUEST[$attrib] = "cn=DHCP,".$_REQUEST["host"];
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
<?php if($attrib == pql_get_define("PQL_ATTR_IPHOSTNUMBER")) { ?>
      <th colspan="3" align="left"><?=$LANG->_('Change IP address')?></th>
<?php } elseif($attrib == pql_get_define("PQL_ATTR_DHCP3_SERVICEDN")) { ?>
      <th colspan="3" align="left"><?=$LANG->_('Change DN')?></th>
<?php } else { ?>
      <th colspan="3" align="left"></th>
<?php } ?>
        <tr class="<?php pql_format_table(); ?>">
<?php if($attrib == pql_get_define("PQL_ATTR_IPHOSTNUMBER")) { ?>
          <td class="title"><?=$LANG->_('IP address')?></td>
<?php } elseif($attrib == pql_get_define("PQL_ATTR_DHCP3_SERVICEDN")) { ?>
          <td class="title"><?=$LANG->_('DN')?></td>
<?php } else { ?>
          <td class="title"></td>
<?php } ?>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?>
            <input type="input" name="<?=$attrib?>" value="<?=$_REQUEST[$attrib]?>"<?php if($attrib == pql_get_define("PQL_ATTR_DHCP3_SERVICEDN")) { ?> size="50"<?php } ?>>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="action"   value="<?=$_REQUEST["action"]?>">
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="oldvalue" value="<?=$_REQUEST["oldvalue"]?>">

    <input type="hidden" name="host"     value="<?=urlencode($_REQUEST["host"])?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(void)
function attribute_save($type) {
  global $_pql, $LANG;
  $attrib = $_REQUEST["attrib"];
  
  switch($type) {
  case "add":
	// no add function for ipHostNumber
	break;
	
  case "modify":
	if($attrib == pql_get_define("PQL_ATTR_IPHOSTNUMBER")) {
	  if(pql_modify_attribute($_REQUEST["host"], $attrib, 1, $_REQUEST[$attrib])) {
		$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									 array('what' => $LANG->_('IP address')));
		if(pql_get_define("PQL_ATTR_BIND9_USE") and ($attrib == pql_get_define("PQL_ATTR_IPHOSTNUMBER"))) {
		  // TODO: Change a DNS record
		  $x = 'x';
		}
	  } else
		$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									 array('what' => $LANG->_('IP address'))) . ": " . pql_format_error(0);
	} elseif($attrib == pql_get_define("PQL_ATTR_DHCP3_SERVICEDN")) {
	  // Setup the physical host object.

	  // Get the current object classes of the object.
	  $ocs   = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_OBJECTCLASS"));

	  // Need to lower-case all object classes for the in_array() to work.
	  $new = array();
	  for($i=0; $ocs[$i]; $i++) {
		$new[] = lc($ocs[$i]);
	  }
	  $ocs = $new;

	  // Add: object class dhcpServer (if it wasn't added when creating the physical host)
	  // Add: object class dhcpService
	  if(!in_array(pql_get_define("PQL_ATTR_DHCP3_SERVICE"), $ocs))
		$ocs[] = pql_get_define("PQL_ATTR_DHCP3_SERVICE");
	  if(!in_array(pql_get_define("PQL_ATTR_DHCP3_SERVER"), $ocs))
		$ocs[] = pql_get_define("PQL_ATTR_DHCP3_SERVER");

	  $entry = array();
	  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = $ocs;
	  $entry[$attrib] = $_REQUEST[$attrib];

	  if($_pql->modify($_REQUEST["host"], $entry, 'attrib.hostdhcp.inc')) {
		$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									 array('what' => $LANG->_('DHCP service DN')));

		// Create the DHCP service object
		$dn = $_REQUEST[$attrib];
		
		$cn = split(',', $_REQUEST[$attrib]);
		$cn = split('=', $cn[0]);
		
		$entry = array();
		$entry[pql_get_define("PQL_ATTR_CN")] = $cn[1];
		$entry[pql_get_define("PQL_ATTR_OBJECTCLASS")] = pql_get_define("PQL_ATTR_DHCP3_SERVICE");
		$entry[pql_get_define("PQL_ATTR_DHCP3_PRIMARYDN")] = urldecode($_REQUEST["host"]);
		
		if($_pql->add($dn, $entry, 'dhcp3', 'attrib.hostdhcp.inc:attribute_save'))
		  $msg .= '<br>'.pql_complete_constant($LANG->_('Successfully added %what%'),
											   array('what' => $LANG->_('DHCP service DN')));
		else
		  $msg .= '<br>'.pql_complete_constant($LANG->_('Failed to add %what%'),
											   array('what' => $LANG->_('DHCP service DN')));
	  } else
		$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									 array('what' => $LANG->_('DHCP service DN'))) . ": " . pql_format_error(0);
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
