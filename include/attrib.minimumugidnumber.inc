<?php
// attribute plugin for
// Minimum UID/GID Number to be used for shell and system accounts for domain

// {{{ attribute_check(void)
function attribute_check() {
    global $error, $LANG;

	$attrib = $_REQUEST["attrib"];
    
    // is typed in ?
    if(empty($_REQUEST[$attrib])) {
		$error[$attrib] = $LANG->_('Missing');
		return false;
    }
    
    // must be integer
    if(!is_numeric($_REQUEST[$attrib]) ||
	   ($_REQUEST[$attrib] < 0) ||
	   ($_REQUEST[$attrib] > 65535))
	{
		$error[$attrib] = $LANG->_('Invalid');
		return false;
    }
    
    return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
	$attrib = $_REQUEST["attrib"];
	$_REQUEST[$attrib] = $_REQUEST["oldvalue"];
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $error, $LANG;

	$attrib = $_REQUEST["attrib"];
	if($attrib == 'minimumuidnumber')
	  $msg = $LANG->_('Minimum UID Number to be used for shell and system accounts');
	elseif($attrib == 'minimumgidnumber')
	  $msg = $LANG->_('Minimum GID Number to be used for shell and system accounts');
?>
  <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Change %what% for domain %domain%'), array('what' => $msg, 'domain' => $_REQUEST["orgname"])); ?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $msg; ?></td>
          <td><?php echo pql_format_error_span($error[$attrib]); ?><input type="text" name="<?php echo $attrib ?>" value="<?php echo $_REQUEST[$attrib]?>" size="7"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="2">
    <input type="hidden" name="attrib"   value="<?php echo $_REQUEST["attrib"]?>">
    <!-- DLW: Check to make sure I didnt set oldvalue to oldvalue in any of the attributes. -->
    <input type="hidden" name="oldvalue" value="<?php echo $_REQUEST[$attrib]?>">
    <input type="hidden" name="domain"   value="<?php echo urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?php echo urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="view"     value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;

	$attrib = $_REQUEST["attrib"];
	if($attrib == 'minimumuidnumber')
	  $msg = $LANG->_('Minimum UID Number to be used for shell and system accounts');
	elseif($attrib == 'minimumgidnumber')
	  $msg = $LANG->_('Minimum GID Number to be used for shell and system accounts');

    switch($type) {
      case "add":
      case "modify":
		if($_REQUEST[$attrib] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $msg));
			attribute_forward($msg);
			break;
    	}

		if(pql_modify_attribute($_REQUEST["domain"], $_REQUEST["attrib"], 1, $_REQUEST[$attrib]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $msg,
											 'new'  => $_REQUEST[$attrib]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $msg)) . ": " . ldap_error($_pql->ldap_linkid);
		
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
