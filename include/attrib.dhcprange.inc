<?php
// attribute for phpQLAdmin/DHCP subnet range configuration
// $Id: attrib.dhcprange.inc,v 2.1 2007-07-19 10:28:00 turbo Exp $

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

	$tmp = explode(' ', $_REQUEST[$attrib]);

	$_REQUEST[$attrib] = $tmp[0];
	$_REQUEST[$attrib."_value"] = $tmp[1];
  }
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Add DHCP subnet range')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Range')?></td>
          <td>
            <?php echo pql_format_error_span($error[$attrib]); ?>
            <input type="input" name="<?php echo $attrib?>" value="<?php echo $_REQUEST[$attrib]?>" size="50">
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"></td>
          <td>
            <input type="checkbox" name="dynbootp">
            &nbsp;<?php echo $LANG->_('Dynamic BOOTP range')?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"    value="1">
    <input type="hidden" name="action"    value="<?php echo $_REQUEST["action"]?>">
    <input type="hidden" name="attrib"    value="<?php echo $attrib?>">
    <input type="hidden" name="oldvalue"  value="<?php echo $_REQUEST["oldvalue"]?>">

    <input type="hidden" name="host"      value="<?php echo urlencode($_REQUEST["host"])?>">
    <input type="hidden" name="subnet"    value="<?php echo urlencode($_REQUEST["subnet"])?>">
    <input type="hidden" name="view"      value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
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
	printr($_REQUEST);

	if($_REQUEST[$attrib]) {
	  $old = $_pql->get_attribute($_REQUEST["subnet"], $attrib);
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

	  if(@$_REQUEST["dynbootp"])
		$value[] = "dynamic-bootp ".$tmp;
	  else
		$value[] = $tmp;
		
	  if(pql_modify_attribute($_REQUEST["subnet"], $attrib, 1, $value))
		$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									 array('what' => $LANG->_('DHCP subnet range')));
	  else
		$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									 array('what' => $LANG->_('DHCP subnet range'))) . ": " . pql_format_error(0);
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
