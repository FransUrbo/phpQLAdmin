<?php
// attribute plugin for
// qmaildotmode (interpretation of .qmail-files)
// $Id: attrib.qmaildotmode.inc,v 2.19.10.1 2006-03-23 09:13:15 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type) {
    global $error, $LANG;
	
	// is typed in ?
	if(empty($_REQUEST["qmaildotmode"])) {
		$error["qmaildotmode"] = $LANG->_('Missing');
		return false;
	}
	
	// valid entry ?
	switch($_REQUEST["qmaildotmode"]) {
	  case "default":
	  case "both":
	  case "dotonly":
	  case "ldaponly":
	  case "ldapwithprog":
	  case "none":
		break;
	  default:
		$error["qmaildotmode"] = $LANG->_('Invalid');
		return false;
	}
	
	return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    $_REQUEST["qmaildotmode"] = $_REQUEST["oldvalue"];
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $error, $LANG;

	$qmaildotmode_select[$_REQUEST["qmaildotmode"]] = "selected";
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Change dotmode')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Change dotmode')?></td>
          <td>
            <?php echo pql_format_error_span($error["qmaildotmode"]); ?>
            <select name="qmaildotmode">
			  <option value="default" <?=@$qmaildotmode_select["default"]?>><?=$LANG->_('Default (taken from ldapDefaultDotMode)')?></option>
			  <option value="both" <?=@$qmaildotmode_select["both"]?>><?=$LANG->_('Both (deliveryProgramPath + .qmail)')?></option>
			  <option value="dotonly" <?=@$qmaildotmode_select["dotonly"]?>><?=$LANG->_('Dotonly (only .qmail)')?></option>
			  <option value="ldaponly" <?=@$qmaildotmode_select["ldaponly"]?>><?=$LANG->_('LDAPOnly (deliveryProgramPath + .qmail ignored for ldap deliveries)')?></option>
			  <option value="ldapwithprog" <?=@$qmaildotmode_select["ldapwithprog"]?>><?=$LANG->_('LDAPWithProg (deliveryProgramPath used, .qmail ignored)')?></option>
			  <option value="none" <?=@$qmaildotmode_select["none"]?>><?=$LANG->_('None (deliveryProgramPath + .qmail ignored)')?></option>
            </select>
          </td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <?php echo pql_format_error_span($error["general"]); ?>
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_('If you set dotmode to both or ldapwithprog you should add a deliveryprogram too')?>
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
		// no add operation possible with qmaildotmode
		break;

	  case "modify":
		if($_REQUEST["qmaildotmode"] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Dotmode')));
			attribute_forward($msg);
			break;
    	}
		
		if($_REQUEST["qmaildotmode"] == "default")
		  $_REQUEST["qmaildotmode"] = "";
		
		if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["user"], pql_get_define("PQL_ATTR_DOTMODE"),
								'', $_REQUEST["qmaildotmode"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('dot mode'),
											 'new'  => $_REQUEST["qmaildotmode"]));
		else {
		  pql_format_error();
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('dot mode')));
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
