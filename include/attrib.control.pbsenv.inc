<?php
// control attribute plugin for
// pbsenv (max number of bytes in message)
//
// $Id: attrib.control.pbsenv.inc,v 2.14 2007-02-15 12:07:40 turbo Exp $

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
	if($host == "Global")
	  $_REQUEST["pbsenv"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
											  pql_get_define("PQL_ATTR_PBS_ENV"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["pbsenv"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_PBS_ENV"));

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
        <td class="title" colspan="3"><?php echo $LANG->_('Additional environment variables.')?></td>
      </tr>

<?php
	$class   = pql_format_table(0);
	$url_tmp = "control_edit_attribute.php?mxhost=$host&attrib=".pql_get_define("PQL_ATTR_PBS_ENV");

	$tbl_txt = array('tit' => $LANG->_('Variable'), 'add' => $LANG->_('Add variable'), 'del' => $LANG->_('Remove variable'));
	$tbl_url = array('add' => $url_tmp.'&type=add', 'del' => $url_tmp.'&type=del&submit=1');
	pql_format_table_single( $tbl_txt, $tbl_url, $_REQUEST["pbsenv"],
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
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
<?php for($i=0; $i < count($_REQUEST["pbsenv"]); $i++) { ?>
    <input type="hidden" name="pbsenv[<?php echo $i?>]" value="<?php echo $_REQUEST["pbsenv"][$i]?>"></td>
<?php } ?>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">pbsenv (<?php echo $LANG->_('Additional environment variables')?>)</th>
        <tr class="title">
          <td><?php echo $LANG->_('Variable name')?></td>
          <td>&nbsp;</td>
          <td><?php echo $LANG->_('Variable value')?></td>
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
    <input type="hidden" name="attrib" value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?php echo $_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ pbsenv_save(type, host)
function pbsenv_save($type, $host)  {
	global $_pql_control, $error, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		for($i=0; $i < count($_REQUEST["pbsenv"]); $i++)
		  $entry[] = $_REQUEST["pbsenv"][$i];
		$entry[] = $_REQUEST["pbsenvariable_name"].'='.$_REQUEST["pbsenvariable_value"];

		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($host,
									 pql_get_define("PQL_ATTR_PBS_ENV"), '', $entry))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_PBS_ENV"), '', $entry))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('pbsenv options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('pbsenv options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
		// }}}

	  case "del":
		// {{{ delete value
		if(pql_modify_attribute($host,
								pql_get_define("PQL_ATTR_PBS_ENV"), $_REQUEST["pbsenv"], ''))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('PBS environment variable')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('')))
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
