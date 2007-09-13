<?php
// attribute plugin for phpQLAdmin configuration
// $Id: attrib.dhcp3.inc,v 1.1 2007-09-13 17:59:17 turbo Exp $

// {{{ attribute_change(entry)
function attribute_change($entry) {
  global $_pql, $LANG;

  if($_REQUEST["action"] == "modify") {
	$action_txt_1 = $LANG->_("change");
	$action_txt_2 = $LANG->_("changed");
  } elseif($_REQUEST["action"] == "delete") {
	$action_txt_1 = $LANG->_("delete");
	$action_txt_2 = $LANG->_("deleted");
  }

  // Do the modification
  if(pql_modify_attribute($_REQUEST["config"], $_REQUEST["attrib"], 1, $entry))
	$msg = pql_complete_constant($LANG->_('Successfully %action% %what%'),
								 array('what'   => $_REQUEST["value"],
									   'action' => $action_txt_2));
  else
	$msg = pql_complete_constant($LANG->_('Failed to %action% %what%'),
								   array('what'   => $_REQUEST["value"],
										 'action' => $action_txt_1));

  return($msg);
}
// }}}

// {{{ attribute_check(void)
function attribute_check() {
  global $error, $LANG;

  $value = $_REQUEST["value"];
  if(!$_REQUEST[$value])
	return 0;

  return 1;
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form($data) {
  global $_pql, $error, $LANG;

  $info = split(' ', $data);

  $value = '';
  for($j=1; $info[$j]; $j++) {
	$info[$j] = preg_replace('/"/', '', $info[$j]);
	$value .= $info[$j];
	if($info[$j+1]) {
	  $value .= " ";
	}
  }
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$info[0]?></td>
          <td>&nbsp;<input type="input" name="<?=$info[0]?>" size="40" value="<?=$value?>"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="value"    value="<?=$info[0]?>">
    <input type="hidden" name="oldvalue" value="<?=urlencode($data)?>">
    <input type="hidden" name="config"   value="<?=urlencode($_REQUEST["config"])?>">
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <input type="hidden" name="action"   value="<?=$_REQUEST["action"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(void)
function attribute_save($type) {
  global $_pql, $LANG;

  $value    = $_REQUEST["value"];
  $attrib   = $_REQUEST["attrib"];
  $newvalue = $_REQUEST["value"]." ".$_REQUEST[$value];

  // Get old value(s)
  $entry = $_pql->get_attribute($_REQUEST["config"], $attrib);

  switch($type) {
  case "modify":
	// Go through values and exchange the old with the new
	for($i=0; $entry[$i]; $i++) {
	  if(ereg("^$value", $entry[$i]))
		$entry[$i] = $newvalue;
	}

	$msg = attribute_change($entry);
	attribute_forward($msg);
	break;
	
  case "delete":
	// The 'value' isn't known with a delete option, get it...
	$tmp = split(' ', $_REQUEST[data]);
	$_REQUEST["value"] = $tmp[0];

	// Go through values and remove the specified value
	for($i=0; $entry[$i]; $i++) {
	  if($_REQUEST["data"] == $entry[$i])
		unset($entry[$i]);
	}
	$entry = pql_uniq($entry);

	$msg = attribute_change($entry);
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
