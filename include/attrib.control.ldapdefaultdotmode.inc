<?php
// control attribute plugin for
// ldapdefaultdotmode (default mode how to interpret .qmail files)
//
// $Id: attrib.control.ldapdefaultdotmode.inc,v 2.14 2004-03-11 16:31:54 turbo Exp $

// {{{ ldapdefaultdotmode_check(type)
function ldapdefaultdotmode_check($type) {
	global $error, $ldapdefaultdotmode, $view, $LANG;
	
	// missing
	if($ldapdefaultdotmode == "") {
		$error["ldapdefaultdotmode"] = $LANG->_('Missing');
		return false;
	}
	
	switch($ldapdefaultdotmode) {
	  case "both":
	  case "dotonly":
	  case "ldaponly":
	  case "ldapwithprog":
	  case "none":
		break;

	  default:
		$error["ldapdefaultdotmode"] = $LANG->_('Invalid');
		return false;
	}
	
	return true;
}
// }}}

// {{{ ldapdefaultdotmode_init(host)
// init values
function ldapdefaultdotmode_init($host) {
	global $_pql_control;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE"));
	
	if(!is_null($value))
	  $_REQUEST["ldapdefaultdotmode"] = $value[0];
}
// }}}


// {{{ ldapdefaultdotmode_print_view(host)
function ldapdefaultdotmode_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	ldapdefaultdotmode_init($host);

	// set empty field to "not set"
	if($_REQUEST["ldapdefaultdotmode"] == "")
	  $_REQUEST["ldapdefaultdotmode"] = $LANG->_('Not set');
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">ldapdefaultdotmode (<?=$LANG->_('Default dot mode')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Mode')?></td>
        <td><?=$_REQUEST["ldapdefaultdotmode"]?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldapdefaultdotmode"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ ldapdefaultdotmode_print_form(void)
function ldapdefaultdotmode_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">ldapdefaultdotmode (<?=$LANG->_('Default dot mode')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Mode')?></td>
          <td><?php echo pql_format_error_span($error["ldapdefaultdotmode"]); ?>
            <select name="ldapdefaultdotmode">
              <option value="ldaponly" <?php if($_REQUEST["ldapdefaultdotmode"] == "ldaponly"){ echo "SELECTED"; }?>>ldaponly (<?=$LANG->_('LDAP attribute "deliveryProgramPath" and .qmail files are ignored')?>)</option>
              <option value="dotonly" <?php if($_REQUEST["ldapdefaultdotmode"] == "dotonly"){ echo "SELECTED"; }?>>dotonly (<?=$LANG->_('only .qmail')?>)</option>
              <option value="ldapwithprog" <?php if($_REQUEST["ldapdefaultdotmode"] == "ldapwithprog"){ echo "SELECTED"; }?>>ldapwithprog (<?=$LANG->_('Attribute deliveryProgramPath is used if existant, .qmail files are ignored')?>))</option>
              <option value="both" <?php if($_REQUEST["ldapdefaultdotmode"] == "both"){ echo "SELECTED"; }?>>both (<?=$LANG->_('LDAP attribute "deliveryProgramPath" and .qmail files are used')?>)</option>
              <option value="none" <?php if($_REQUEST["ldapdefaultdotmode"] == "none"){ echo "SELECTED"; }?>>none (<?=$LANG->_('Both LDAP attribute and .qmail files are ignored')?>)</option>
            </select>
          </td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('This affects LDAP delivery only. It doesn\'t work for local non-ldap deliveries')?>.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?=$_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ ldapdefaultdotmode_save(type, host)
function ldapdefaultdotmode_save($type, $host) {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE"),
										 $_REQUEST["ldapdefaultdotmode"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldapdefaultdotmode options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldapdefaultdotmode options')))
			. ": " . pql_format_error(0);

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
