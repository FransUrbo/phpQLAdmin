<?php
// attribute plugin for
// qmaildotmode (interpretation of .qmail-files)
// $Id: attrib.qmaildotmode.inc,v 2.23 2007-02-26 12:42:26 turbo Exp $

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

	// {{{ Get default dot mode for servers
	// Get all hosts with ldapDefaultDotMode defined
	require($_SESSION["path"]."/include/pql_control.inc");
	$_pql_control = new pql_control($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
	$qlcs = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
								  '(&(cn=*)(objectclass=qmailControl)(ldapDefaultDotMode=*))');

	// Take the value from the first - should be enough (?)
	$default_dot_mode = $_pql_control->get_attribute($qlcs[0], pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE"));
	// }}}
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Change dotmode')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Change dotmode')?></td>
          <td>
            <?php echo pql_format_error_span($error["qmaildotmode"]); ?>
            <select name="qmaildotmode">
			  <option value="default" <?php echo @$qmaildotmode_select["default"]?>><?php echo $LANG->_('Default (taken from ldapDefaultDotMode)')?></option>
			  <option value="both" <?php echo @$qmaildotmode_select["both"]?>><?php echo $LANG->_('Both (deliveryProgramPath + .qmail)')?></option>
			  <option value="dotonly" <?php echo @$qmaildotmode_select["dotonly"]?>><?php echo $LANG->_('Dotonly (only .qmail)')?></option>
			  <option value="ldaponly" <?php echo @$qmaildotmode_select["ldaponly"]?>><?php echo $LANG->_('LDAPOnly (deliveryProgramPath + .qmail ignored for ldap deliveries)')?></option>
			  <option value="ldapwithprog" <?php echo @$qmaildotmode_select["ldapwithprog"]?>><?php echo $LANG->_('LDAPWithProg (deliveryProgramPath used, .qmail ignored)')?></option>
			  <option value="none" <?php echo @$qmaildotmode_select["none"]?>><?php echo $LANG->_('None (deliveryProgramPath + .qmail ignored)')?></option>
            </select>
          </td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <?php echo pql_format_error_span($error["general"]); ?>
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo $LANG->_('If you set dotmode to both or ldapwithprog you should add a deliveryprogram too')?>.
          </td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo $LANG->_('Default dot mode is:')?>&nbsp;<b><?php echo $default_dot_mode?></b>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?php echo $_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"   value="<?php echo urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?php echo urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"     value="<?php echo urlencode($_REQUEST["user"])?>">
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
		
		if(pql_modify_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_DOTMODE"),
								'', $_REQUEST["qmaildotmode"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
									   array('what' => $LANG->_('dot mode'),
											 'new'  => $_REQUEST["qmaildotmode"]));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('dot mode'))) . ": " . pql_format_error(0);
		
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
