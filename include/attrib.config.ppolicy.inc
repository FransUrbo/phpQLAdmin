<?php
// attribute plugin for phpQLAdmin/Password Policies
// $Id: attrib.config.ppolicy.inc,v 2.1 2007-03-05 10:12:25 turbo Exp $

// {{{ attribute_check_ppolicy(void)
function attribute_check_ppolicy() {
  global $LANG;
  $error = false;
  $error_text = array();

  if($_REQUEST["submit"]) {
	if(!$_REQUEST[pql_get_define("PQL_ATTR_PPOLICY_NAME")]) {
	  $error = true;
	  $error_text[pql_get_define("PQL_ATTR_PPOLICY_NAME")] = $LANG->_('Missing');
	}
	
	if(!$_REQUEST[pql_get_define("PQL_ATTR_PPOLICY_DESC")]) {
	  $error = true;
	  $error_text[pql_get_define("PQL_ATTR_PPOLICY_DESC")] = $LANG->_('Missing');
	}
  } else
	$error = true;

  if($error)
	return($error_text);
}
// }}}

// {{{ attribute_print_form_ppolicy(errors)
function attribute_print_form_ppolicy($error_text) {
  global $LANG, $table_columns;
?>
  <span class="title1"><?=$LANG->_('Create a password policy')?></span>

  <br><br>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Add password policy'); ?>
<?php foreach($table_columns as $attrib => $text) {
	  $link = 'config_edit_attribute.php?attrib='.$attrib;
	  $class=pql_format_table(0);
?>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$text?></td>
        <td>
          <?=pql_format_error_span($error_text[$attrib]); ?>
<?php	if(($attrib == pql_get_define("PQL_ATTR_PPOLICY_NAME")) or
		   ($attrib == pql_get_define("PQL_ATTR_PPOLICY_DESC")) or
		   ($attrib == pql_get_define("PQL_ATTR_PPOLICY_CHECK_MODULE")) or
		   ($attrib == pql_get_define("PQL_ATTR_PPOLICY_INHIST")) or
		   ($attrib == pql_get_define("PQL_ATTR_PPOLICY_MINLEN")) or
		   ($attrib == pql_get_define("PQL_ATTR_PPOLICY_GRACEAUTHLIMIT")) or
		   ($attrib == pql_get_define("PQL_ATTR_PPOLICY_MAXFAILURE"))) {
			// String/Integer value

			if(($attrib == pql_get_define("PQL_ATTR_PPOLICY_NAME")) or
			   ($attrib == pql_get_define("PQL_ATTR_PPOLICY_DESC"))) {
				// Required value
?>
          <font color="red">*</font>
<?php		} else { ?>
          &nbsp;&nbsp;
<?php		} ?>
          <input type="text" name="<?=$attrib?>" size="20" value="<?=$_REQUEST[$attrib]?>">
<?php	} elseif(($attrib == pql_get_define("PQL_ATTR_PPOLICY_MINAGE")) or
				 ($attrib == pql_get_define("PQL_ATTR_PPOLICY_MAXAGE")) or
				 ($attrib == pql_get_define("PQL_ATTR_PPOLICY_EXPIRE_WARNING")) or
				 ($attrib == pql_get_define("PQL_ATTR_PPOLICY_LOCKOUT_DURATION")) or
				 ($attrib == pql_get_define("PQL_ATTR_PPOLICY_FAILURE_COUNT_INTERVAL"))) {
			// Seconds value
?>
          &nbsp;&nbsp;
          <input type="text" name="<?=$attrib?>" size="9" value="<?=$_REQUEST[$attrib]?>">
          <select name="<?=$attrib?>_time">
            <option value="seconds" SELECTED><?=$LANG->_('Seconds')?></option>
            <option value="minutes"><?=$LANG->_('Minutes')?></option>
            <option value="hours"><?=$LANG->_('Hours')?></option>
            <option value="days"><?=$LANG->_('Days')?></option>
            <option value="weeks"><?=$LANG->_('Weeks')?></option>
            <option value="months"><?=$LANG->_('Months')?></option>
            <option value="years"><?=$LANG->_('Years')?></option>
          </select>
<?php	} elseif(($attrib == pql_get_define("PQL_ATTR_PPOLICY_LOCKOUT")) or
				 ($attrib == pql_get_define("PQL_ATTR_PPOLICY_MUST_CHANGE")) or
				 ($attrib == pql_get_define("PQL_ATTR_PPOLICY_ALLOW_USER_CHANGE")) or
				 ($attrib == pql_get_define("PQL_ATTR_PPOLICY_SAFE_MODIFY"))) {
			// Boolean value
?>
          &nbsp;&nbsp;
          <select name="<?=$attrib?>">
            <option value="TRUE" SELECTED><?=$LANG->_('TRUE')?></option>
            <option value="FALSE"><?=$LANG->_('FALSE')?></option>
          </select>
<?php	} elseif($attrib == pql_get_define("PQL_ATTR_PPOLICY_CHKQUAL")) {
			// enum(0,1,2) value
?>
          &nbsp;<input type="radio" name="<?=$attrib?>" value="0"><?=$LANG->_('No syntax checking')?><br>
          &nbsp;<input type="radio" name="<?=$attrib?>" value="1"><?=$LANG->_('Server checks syntax - return true if failed to check password')?><br>
          &nbsp;<input type="radio" name="<?=$attrib?>" value="2" CHECKED><?=$LANG->_('Server checks syntax - return false if failed to check password')?><br>
<?php	} elseif($attrib == pql_get_define("PQL_ATTR_PPOLICY_ATTR")) { ?>
          <font color="red">*</font>
          userPassword (harcoded)
          <input type="hidden" name="<?=$attrib?>" value="userPassword">
<?php	} ?>
        </td>
      </tr>
<?php } ?>
      </th>
    </table>

    <input type="hidden" name="submit" value="submit">
    <input type="hidden" name="action" value="add">
    <input type="hidden" name="rootdn" value="<?=urlencode($_REQUEST["rootdn"])?>">
    <br>
    <input type="submit" value="Create">
  </form>
<?php  
}
// }}}

// {{{ attribute_save_ppolicy(void)
function attribute_save_ppolicy() {
  global $_pql, $table_columns, $LANG;
  
  // {{{ Create the organizationalUnit leading up to the templates...
  if(!pql_unit_add($_REQUEST["rootdn"], pql_get_define("PQL_CONF_SUBTREE_PPOLICY")))
	die("pql_write_add(ou): failure<br>");
  // }}}

  // {{{ Create the template object 'LDIF'
  $entry = array();
  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'pwdPolicy';
  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'phpQLAdminPasswordPolicy';
  foreach($table_columns as $attrib => $text) {
	if($_REQUEST[$attrib] and
	   (($attrib == pql_get_define("PQL_ATTR_PPOLICY_MINAGE")) or
		($attrib == pql_get_define("PQL_ATTR_PPOLICY_MAXAGE")) or
		($attrib == pql_get_define("PQL_ATTR_PPOLICY_EXPIRE_WARNING")) or
		($attrib == pql_get_define("PQL_ATTR_PPOLICY_LOCKOUT_DURATION")) or
		($attrib == pql_get_define("PQL_ATTR_PPOLICY_FAILURE_COUNT_INTERVAL")))) {
	  // Seconds value - special case
	  $newvalue = $_REQUEST[$attrib];

	  // {{{ Calculate number of seconds
	  if($_REQUEST[$attrib."_time"] == "minutes")
		$newvalue = $newvalue * 60;
	  elseif($_REQUEST[$attrib."_time"] == "hours")
		$newvalue = $newvalue * 60 * 60;
	  elseif($_REQUEST[$attrib."_time"] == "days")
		$newvalue = $newvalue * 60 * 60 * 24;
	  elseif($_REQUEST[$attrib."_time"] == "weeks")
		$newvalue = $newvalue * 60 * 60 * 24 * 7;
	  elseif($_REQUEST[$attrib."_time"] == "months")
		$newvalue = $newvalue * 60 * 60 * 24 * 7 * 4;
	  elseif($_REQUEST[$attrib."_time"] == "years")
		$newvalue = $newvalue * 60 * 60 * 24 * 7 * 4 * 12;
	  // }}}

	  $entry[$attrib] = $newvalue;
	} elseif($_REQUEST[$attrib]) {
	  // Other value - use as is
	  $entry[$attrib] = $_REQUEST[$attrib];
	}
  }
  // }}}

  // {{{ Generate the DN
  $dn  = pql_get_define("PQL_ATTR_PPOLICY_NAME")."=".lc($_REQUEST[pql_get_define("PQL_ATTR_PPOLICY_NAME")]);
  $dn .= ",".pql_get_define("PQL_CONF_SUBTREE_PPOLICY").",".$_REQUEST["rootdn"];
  // }}}

  // {{{ Add the template to the database.
  if(!$_pql->add($dn, $entry, 'ppolicy', 'config_ppolicy_add/ppolicy'))
	$msg = pql_complete_constant($LANG->_("Failed to create password policy %policy%"),
								 array('policy' => $_REQUEST[pql_get_define("PQL_ATTR_PPOLICY_NAME")]));
  else
	$msg = pql_complete_constant($LANG->_("Successfully created password policy %policy%"),
								 array('policy' => $_REQUEST[pql_get_define("PQL_ATTR_PPOLICY_NAME")]));
  // }}}

  attribute_forward($msg);
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
