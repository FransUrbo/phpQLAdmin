<?php
// attribute plugin for
// deliveryprogrampath (path to program running on delivery)
// $Id: attrib.deliveryprogrampath.inc,v 2.20 2006-12-16 12:02:14 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type) {
    global $error, $LANG;
	
	// trailing slash ?
	if (!empty($_REQUEST["deliveryprogrampath"]) and !preg_match("/^(\/)/", $_REQUEST["deliveryprogrampath"])) {
		$error["deliveryprogrampath"] = $LANG->_('Invalid');
		return false;
	}
	
	return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    global $_pql;

    $deliveryprogrampath = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_PROGRAM"));
	$_REQUEST["deliveryprogrampath"] = $deliveryprogrampath[0];

    // DLW: Why is "oldvalue" getting assigned a value here?
	$_REQUEST["oldvalue"] = $_REQUEST["deliveryprogrampath"];
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change path to deliveryprogram')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Change path to deliveryprogram')?></td>
          <td><?php echo pql_format_error_span($error["deliveryprogrampath"]); ?><input type="text" name="deliveryprogrampath" value="<?=$_REQUEST["deliveryprogrampath"]?>"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <?php echo pql_format_error_span($error["general"]); ?>
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_('This is the same as \'|/path/to/someprog\' in a .qmail file. Only used if qmailDotMode is set to ldapwithprog or both')?>.
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?=$_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"   value="<?=urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?=urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"     value="<?=urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;
	
	switch($type) {
	  case "add":
		// no add operation possible with deliveryprogrampath
		break;

	  case "modify":
		if($_REQUEST["deliveryprogrampath"] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Delivery program path')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_PROGRAM"),
								'', $_REQUEST["deliveryprogrampath"])) {
			if($_REQUEST["deliveryprogrampath"] == "")
			  $_REQUEST["deliveryprogrampath"] = $LANG->_('None');
			
			$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										 array('what' => $LANG->_('path'),
											   'new'  => $_REQUEST["deliveryprogrampath"]));
		} else {
			$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										 array('what' => $LANG->_('path'))) . ": " . pql_format_error(0);
		}

		// DLW: This shouldn't care if deliveryprogrampath gets set to None.
		// check if qmaildotmode is set to 'both' or 'ldapwithprog'
		$qmaildotmode = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_DOTMODE"));
		$qmaildotmode = $qmaildotmode[0];

		if($qmaildotmode != "both" and $qmaildotmode != "ldapwithprog")
		  $msg .= "<br>" . pql_complete_constant($LANG->_('The qmaildotmode of this user is set to %qmaildotmode%. Please to set it to both or ldapWithProg to get the deliveryprogram running'),
												 array("qmaildotmode" => $qmaildotmode));
		
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
