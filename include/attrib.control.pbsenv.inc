<?php
// control attribute plugin for
// pbsenv (max number of bytes in message)
//
// $Id: attrib.control.pbsenv.inc,v 2.7 2005-01-29 18:46:57 turbo Exp $

// {{{ pbsenv_check(type)
function pbsenv_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ pbsenv_init(host)
// init values
function pbsenv_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["pbsenv"] = pql_get_attribute($_pql_control->ldap_linkid,
											pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											pql_get_define("PQL_ATTR_PBS_ENV"));

	if(is_null($_REQUEST["pbsenv"]))
	  return true;
}
// }}}


// {{{ pbsenv_print_view(host)
function pbsenv_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	pbsenv_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">pbsenv (<?=$LANG->_('Additional environment variables.')?>)</th>
      <tr class="title">
        <td>IP</td>
      </tr>
<?php
	if(!is_array($_REQUEST["pbsenv"])) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Not set')?></td>
      </tr>
<?php
	} else {
		foreach($_REQUEST["pbsenv"] as $variable) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=pql_maybe_idna_decode($variable)?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=pbsenv"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ pbsenv_print_form(void)
function pbsenv_print_form() {
	global $error, $LANG;

	if(($_REQUEST["variablecount"] < count($_REQUEST["pbsenv"]) - 1) and count($_REQUEST["pbsenvariable_name"]) != 0)
	  $_REQUEST["variablecount"] = count($_REQUEST["pbsenv"]) - 1;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">pbsenv (<?=$LANG->_('Additional environment variables')?>)</th>
        <tr class="title">
          <td><?=$LANG->_('Variable name')?></td>
          <td>&nbsp;</td>
          <td><?=$LANG->_('Variable value')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["pbsenv_0"]); ?><input type="text" name="pbsenvariable_name[0]" value="<?=$_REQUEST["pbsenvariable_name"][0]?>"></td>
          <td><b>=</b></td>
          <td><?php echo pql_format_error_span($error["pbsenv_0"]); ?><input type="text" name="pbsenvariable_value[0]" value="<?=$_REQUEST["pbsenvariable_value"][0]?>"></td>
        </tr>

<?php for($i = 1; $i <= $_REQUEST["variablecount"]; $i++) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["pbsenv_$i"]); ?><input type="text" name="pbsenvariable_name[<?=$i?>]" value="<?=$_REQUEST["pbsenvariable_name"][$i]?>"></td>
          <td><b>=</b></td>
          <td><?php echo pql_format_error_span($error["pbsenv_$i"]); ?><input type="text" name="pbsenvariable_value[<?=$i?>]" value="<?=$_REQUEST["pbsenvariable_value"][$i]?>"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td colspan="3"><a href="<?$_SERVER["PHP_SELF"]?>?attrib=<?=$_REQUEST["attrib"]?>&variablecount=<?=($_REQUEST["variablecount"]+1)?>&mxhost=<?=$_REQUEST["mxhost"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional variable'))); ?></a> (<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
        </tr>

        <tr class="subtitle">
          <td colspan="3"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('To delete a variable, remove it and save')?>.</td>
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

// {{{ pbsenv_save(type, host)
function pbsenv_save($type, $host)  {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify":
	  case "add":
		// save entries
		for($i=0; $_REQUEST["pbsenvariable_name"][$i] and $_REQUEST["pbsenvariable_value"][$i]; $i++)
		  $entry[] = $_REQUEST["pbsenvariable_name"][$i] . "=" . $_REQUEST["pbsenvariable_value"][$i];

		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_PBS_ENV"), '', $entry))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('pbsenv options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('pbsenv options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ pbsenv_help(void)
function pbsenv_help() {
	global $LANG;
?>
    Additional environment variables to include.
    pbscheck will set these variables to the values pbsadd returned.
    It is possible to rewrite the variables. A line like
    <b>USER=TCPREMOTEINFO</b> will cause pbscheck to set the environment <b>TCPREMOTEINFO</b>
    to the value set as <b>USER</b> in pbsadd. If you like to prepend some identifier
    in front of the value you can this like this: <b>USER=TCPREMOTEINFO=[pbs]</b>.
    This will add <b>[pbs]</b> in front of the value returned by pbsadd.
<?php
}
// }}}

// {{{ pbsenv_help_cr(void)
function pbsenv_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
