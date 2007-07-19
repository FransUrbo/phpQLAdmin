<?php
// attribute for phpQLAdmin/DHCP Option configuration
// $Id: attrib.dhcpoption.inc,v 2.1 2007-07-19 10:28:00 turbo Exp $

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
  if($_REQUEST[$attrib]) {
    $_REQUEST["oldvalue"] = $_REQUEST[$attrib];

	$tmp = split(' ', $_REQUEST[$attrib]);

	$_REQUEST[$attrib] = $tmp[0];
	$_REQUEST[$attrib."_value"] = $tmp[1];
  }
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Add DHCP Statement')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Statement')?></td>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?>
            <select name="<?=$attrib?>">
              <option value="domain-name"<?php if($_REQUEST[$attrib] == "domain-name") { echo " SELECTED"; }?>>domain-name</option>
              <option value="domain-name-servers"<?php if($_REQUEST[$attrib] == "domain-name-servers") { echo " SELECTED"; }?>>domain-name-servers</option>
              <option value="broadcast-address"<?php if($_REQUEST[$attrib] == "broadcast-address") { echo " SELECTED"; }?>>broadcast-address</option>
              <option value="routers"<?php if($_REQUEST[$attrib] == "routers") { echo " SELECTED"; }?>>routers</option>
            </select>
            <input type="input" name="<?=$attrib?>_value" value="<?=$_REQUEST[$attrib."_value"]?>">
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"    value="1">
    <input type="hidden" name="action"    value="<?=$_REQUEST["action"]?>">
    <input type="hidden" name="attrib"    value="<?=$attrib?>">
    <input type="hidden" name="oldvalue"  value="<?=$_REQUEST["oldvalue"]?>">

    <input type="hidden" name="host"      value="<?=urlencode($_REQUEST["host"])?>">
    <input type="hidden" name="servicedn" value="<?=urlencode($_REQUEST["servicedn"])?>">
    <input type="hidden" name="view"      value="<?=$_REQUEST["view"]?>">
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
	if($_REQUEST[$attrib] and $_REQUEST[$attrib."_value"]) {
	  $old = $_pql->get_attribute($_REQUEST["servicedn"], $attrib);
	  if($old and !is_array($old))
		$old = array($old);

	  $tmp = $_REQUEST[$attrib];
	  if($_REQUEST[$attrib."_value"]) {
		if($_REQUEST[$attrib] == "domain-name")
		  $tmp .= ' "'.$_REQUEST[$attrib."_value"]."'";
		else
		  $tmp .= ' '.$_REQUEST[$attrib."_value"];
	  }

	  if(is_array($old))
		$value = $old;
	  else
		$value = array();
	  $value[] = $tmp;
		
	  if(pql_modify_attribute($_REQUEST["servicedn"], $attrib, 1, $value))
		$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									 array('what' => $LANG->_('DHCP Statement')));
	  else
		$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									 array('what' => $LANG->_('DHCP Statement'))) . ": " . pql_format_error(0);
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
