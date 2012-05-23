<?php
// control attribute plugin for
// ldapdefaultdotmode (default mode how to interpret .qmail files)
//
// $Id: attrib.control.ldapdefaultdotmode.inc,v 2.23 2007-02-15 12:07:23 turbo Exp $

// {{{ ldapdefaultdotmode_check(type)
function ldapdefaultdotmode_check($type) {
	global $error, $LANG;
	
	// missing
	if($_REQUEST["ldapdefaultdotmode"] == "") {
		$error["ldapdefaultdotmode"] = $LANG->_('Missing');
		return false;
	}
	
	switch($_REQUEST["ldapdefaultdotmode"]) {
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
	if($host == "Global")
	  $_REQUEST["ldapdefaultdotmode"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
														  pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["ldapdefaultdotmode"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE"));
	
	if(@is_null($_REQUEST["ldapdefaultdotmode"]))
	  return true;
}
// }}}


// {{{ ldapdefaultdotmode_print_view(host)
function ldapdefaultdotmode_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	ldapdefaultdotmode_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">ldapdefaultdotmode (<?php echo $LANG->_('Default dot mode')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?php echo $LANG->_('Mode')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["ldapdefaultdotmode"]) { echo $_REQUEST["ldapdefaultdotmode"]; } else { echo '<i>'.$LANG->_('Not set').'</i>'; }?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options').' '.pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE")?>"></a></td>
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
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">ldapdefaultdotmode (<?php echo $LANG->_('Default dot mode')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Mode')?></td>
          <td><?php echo pql_format_error_span($error["ldapdefaultdotmode"]); ?>
            <select name="ldapdefaultdotmode">
              <option value="ldaponly" <?php if($_REQUEST["ldapdefaultdotmode"] == "ldaponly"){ echo "SELECTED"; }?>>ldaponly (<?php echo $LANG->_('LDAP attribute "deliveryProgramPath" and .qmail files are ignored')?>)</option>
              <option value="dotonly" <?php if($_REQUEST["ldapdefaultdotmode"] == "dotonly"){ echo "SELECTED"; }?>>dotonly (<?php echo $LANG->_('only .qmail')?>)</option>
              <option value="ldapwithprog" <?php if($_REQUEST["ldapdefaultdotmode"] == "ldapwithprog"){ echo "SELECTED"; }?>>ldapwithprog (<?php echo $LANG->_('Attribute deliveryProgramPath is used if existant, .qmail files are ignored')?>))</option>
              <option value="both" <?php if($_REQUEST["ldapdefaultdotmode"] == "both"){ echo "SELECTED"; }?>>both (<?php echo $LANG->_('LDAP attribute "deliveryProgramPath" and .qmail files are used')?>)</option>
              <option value="none" <?php if($_REQUEST["ldapdefaultdotmode"] == "none"){ echo "SELECTED"; }?>>none (<?php echo $LANG->_('Both LDAP attribute and .qmail files are ignored')?>)</option>
            </select>
          </td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?php echo $LANG->_("This affects LDAP delivery only. It doesnt work for local non-ldap deliveries")?>.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="type"   value="modify">
    <input type="hidden" name="attrib" value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?php echo $_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ ldapdefaultdotmode_save(type, host)
function ldapdefaultdotmode_save($type, $host) {
	global $_pql_control, $error, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		if($host == "Global") {
			// Get all QLC host
			$qlcs = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"], '(&(cn=*)(objectclass=qmailControl))');

			$success = 1;
			foreach($qlcs as $host) {
				if(!pql_modify_attribute($host,
										 pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE"), '', $_REQUEST["ldapdefaultdotmode"]))
	 				$success = 0;
			}
		} else {
				if(pql_modify_attribute($host,
									pql_get_define("PQL_ATTR_LDAPDEFAULTDOTMODE"), 1, $_REQUEST["ldapdefaultdotmode"]))
				$success = 1;
		}

		if($success) 
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldapdefaultdotmode options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldapdefaultdotmode options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
		// }}}

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
