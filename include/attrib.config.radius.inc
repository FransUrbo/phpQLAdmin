<?php
// attribute plugin for phpQLAdmin/RADIUS Profiles - global entries
// $Id: attrib.config.radius.inc,v 1.2 2007-09-29 21:15:12 turbo Exp $

// {{{ attribute_check_radprofile(void)
function attribute_check_radprofile() {
  global $LANG;
  $error = false;
  $error_text = array();

  if($_REQUEST["submit"] and !$_REQUEST[pql_get_define("PQL_ATTR_CN")]) {
	$error = true;
	$error_text[pql_get_define("PQL_ATTR_CN")] = $LANG->_('Missing');
  } else
	$error = true;

  if($error)
	return($error_text);
}
// }}}

// {{{ attribute_print_form_radprofile(errors)
function attribute_print_form_radprofile($error_text) {
  global $LANG, $table_columns;
?>
  <span class="title1"><?=$LANG->_('Create a RADIUS profile')?></span>

  <br><br>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Add RADIUS profile'); ?>
<?php foreach($table_columns as $attrib => $text) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$text?></td>
        <td>
          <?=pql_format_error_span($error_text[$attrib]); ?>
<?php	if(($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_NAME"))) or
		   ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_DESC"))) or
		   ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_CHECK_MODULE"))) or
		   ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_INHIST"))) or
		   ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_MINLEN"))) or
		   ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_GRACEAUTHLIMIT"))) or
		   ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_MAXFAILURE"))))
		{
		  // String/Integer value
		  if(($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_NAME"))) or
			 ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_DESC"))))
		  {
			// Required value
			echo "          <font color=\"red\">*</font>\n";
		  } else {
			echo "          &nbsp;&nbsp;\n";
		  }
		  
		  if($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_MINLEN"))) {
?>
          <input type="text" name="<?=$attrib?>" size="7" value="<?=$_REQUEST[$attrib]?>">&nbsp;<?=$LANG->_('characters')?>
<?php	  } else { ?>
          <input type="text" name="<?=$attrib?>" size="20" value="<?=$_REQUEST[$attrib]?>">
<?php	  } ?>
<?php	} elseif(($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_MINAGE"))) or
				 ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_MAXAGE"))) or
				 ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_EXPIRE_WARNING"))) or
				 ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_LOCKOUT_DURATION"))) or
				 ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_FAILURE_COUNT_INTERVAL"))))
		{
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
<?php	} elseif(($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_LOCKOUT"))) or
				 ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_MUST_CHANGE"))) or
				 ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_ALLOW_USER_CHANGE"))) or
				 ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_SAFE_MODIFY")))) {
			// Boolean value
?>
          &nbsp;&nbsp;
          <select name="<?=$attrib?>">
            <option value="TRUE" SELECTED><?=$LANG->_('TRUE')?></option>
            <option value="FALSE"><?=$LANG->_('FALSE')?></option>
          </select>
<?php	} elseif($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_CHKQUAL"))) {
			// enum(0,1,2) value
?>
          &nbsp;<input type="radio" name="<?=$attrib?>" value="0"><?=$LANG->_('No syntax checking')?><br>
          &nbsp;<input type="radio" name="<?=$attrib?>" value="1"><?=$LANG->_('Server checks syntax - return true if failed to check password')?><br>
          &nbsp;<input type="radio" name="<?=$attrib?>" value="2" CHECKED><?=$LANG->_('Server checks syntax - return false if failed to check password')?><br>
<?php	} elseif($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_ATTR"))) { ?>
          <font color="red">*</font>
          userPassword (harcoded)
          <input type="hidden" name="<?=$attrib?>" value="userPassword">
<?php	} else { ?>

          <input type="text" name="<?=$attrib?>" size="20" value="<?=$_REQUEST[$attrib]?>">
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

// {{{ attribute_save_radprofile(void)
function attribute_save_radprofile() {
  global $_pql, $table_columns, $LANG;

  // {{{ Create the organizationalUnit leading up to the templates...
  if(!pql_get_define("PQL_CONF_SUBTREE_PPOLICY"))
	die('The PQL_CONF_SUBTREE_PPOLICY is not set! Please change the include/config.inc file.');

  if(!pql_unit_add($_REQUEST["rootdn"], pql_get_define("PQL_CONF_SUBTREE_PPOLICY")))
	die("pql_write_add(ou): failure<br>");
  // }}}

  // {{{ Create the template object 'LDIF'

  $entry = array();
  $entry[pql_get_define("PQL_ATTR_OBJECTCLASS")][] = 'radiusProfile';
  foreach($table_columns as $attrib => $text) {
	if($_REQUEST[$attrib] and
	   (($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_MINAGE"))) or
		($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_MAXAGE"))) or
		($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_EXPIRE_WARNING"))) or
		($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_LOCKOUT_DURATION"))) or
		($attrib == sprintf("%s", pql_get_define("PQL_ATTR_RADIUS_FAILURE_COUNT_INTERVAL")))))
	{
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

  if(pql_get_define("PQL_ATTR_ACI_USE") and function_exists("user_generate_aci")) {
	$entry[pql_get_define("PQL_ATTR_LDAPACI")] = user_generate_aci($_SESSION["USER_DN"], 'radius');
  }

  // }}}

  // {{{ Generate the DN
  $dn  = pql_get_define("PQL_ATTR_RADIUS_NAME")."=".lc($_REQUEST[pql_get_define("PQL_ATTR_RADIUS_NAME")]);
  $dn .= ",".pql_get_define("PQL_CONF_SUBTREE_PPOLICY").",".$_REQUEST["rootdn"];
  // }}}

  // {{{ Add the template to the database.
  if(!$_pql->add($dn, $entry, 'radius', 'config_radius_add/radius'))
	$msg = pql_complete_constant($LANG->_("Failed to create RADIUS profile %profile%"),
								 array('profile' => $_REQUEST[pql_get_define("PQL_ATTR_RADIUS_NAME")]));
  else
	$msg = pql_complete_constant($LANG->_("Successfully created RADIUS profile %profile%"),
								 array('profile' => $_REQUEST[pql_get_define("PQL_ATTR_RADIUS_NAME")]));
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
