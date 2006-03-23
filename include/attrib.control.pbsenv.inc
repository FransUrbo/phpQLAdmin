<?php
// control attribute plugin for
// pbsenv (max number of bytes in message)
//
// $Id: attrib.control.pbsenv.inc,v 2.10.2.1 2006-03-23 09:13:15 turbo Exp $

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

	if(@is_null($_REQUEST["pbsenv"]) or !@$_REQUEST["pbsenv"][0])
	  return true;
	elseif(!@is_array($_REQUEST["pbsenv"]))
	  $_REQUEST["pbsenv"] = array($_REQUEST["pbsenv"]);
}
// }}}


// {{{ pbsenv_print_view(host)
function pbsenv_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	pbsenv_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">pbsenv
      <tr>
        <td class="title" colspan="3"><?=$LANG->_('Additional environment variables.')?></td>
      </tr>

<?php
	$class   = pql_format_table(0);
	$url_tmp = "control_edit_attribute.php?mxhost=$host&attrib=".pql_get_define("PQL_ATTR_PBS_ENV");

	$tbl_txt = array('tit' => $LANG->_('Variable'), 'add' => $LANG->_('Add variable'), 'del' => $LANG->_('Remove variable'));
	$tbl_url = array('add' => $url_tmp.'&type=add', 'del' => $url_tmp.'&type=del&submit=1');
	pql_format_table_single($_pql_control->ldap_linkid, $tbl_txt, $tbl_url, $_REQUEST["pbsenv"],
							pql_get_define("PQL_ATTR_PBS_ENV"), $class);
?>
    </th>
  </table>

<?php
}
// }}}

// {{{ pbsenv_print_form(void)
function pbsenv_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
<?php for($i=0; $i < count($_REQUEST["pbsenv"]); $i++) { ?>
    <input type="hidden" name="pbsenv[<?=$i?>]" value="<?=$_REQUEST["pbsenv"][$i]?>"></td>
<?php } ?>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">pbsenv (<?=$LANG->_('Additional environment variables')?>)</th>
        <tr class="title">
          <td><?=$LANG->_('Variable name')?></td>
          <td>&nbsp;</td>
          <td><?=$LANG->_('Variable value')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td>
            <?php echo pql_format_error_span($error["pbsenvariable_name"]); ?>
            <input type="text" name="pbsenvariable_name">
          </td>
          <td><b>=</b></td>
          <td>
            <?php echo pql_format_error_span($error["pbsenvariable_value"]); ?>
            <input type="text" name="pbsenvariable_value">
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="type"   value="modify">
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
		for($i=0; $i < count($_REQUEST["pbsenv"]); $i++)
		  $entry[] = $_REQUEST["pbsenv"][$i];
		$entry[] = $_REQUEST["pbsenvariable_name"].'='.$_REQUEST["pbsenvariable_value"];

		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_PBS_ENV"), '', $entry))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('pbsenv options')));
		else {
		  pql_format_error();
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('pbsenv options')));
		}

		attribute_forward($msg);
		break;

	  case "del":
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_PBS_ENV"), $_REQUEST["pbsenv"], ''))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('PBS environment variable')));
		else {
		  pql_format_error();
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('')));
		}

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
