<?php
// control attribute plugin for
// pbstimeout (max number of bytes in message)
//
// $Id: attrib.control.pbstimeout.inc,v 2.12 2007-02-15 12:07:55 turbo Exp $

// {{{ pbstimeout_check(type)
function pbstimeout_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ pbstimeout_init(host)
// init values
function pbstimeout_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["pbstimeout"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
												  pql_get_define("PQL_ATTR_PBS_TIMEOUT"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["pbstimeout"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_PBS_TIMEOUT"));

	if(@is_null($_REQUEST["pbstimeout"]))
	  return true;
}
// }}}


// {{{ pbstimeout_print_view(host)
function pbstimeout_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	pbstimeout_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">pbstimeout (<?php echo $LANG->_('POP before SMTP server timeout')?>)
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Seconds')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["pbstimeout"]) { echo $_REQUEST["pbstimeout"]; } else { echo '<i>'.$LANG->_('Not set').'</i>'; }?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_PBS_TIMEOUT")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options').' '.pql_get_define("PQL_ATTR_PBS_TIMEOUT")?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ pbstimeout_print_form(void)
function pbstimeout_print_form() {
	global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="2" align="left">pbstimeout (<?php echo $LANG->_('POP before SMTP server timeout')?>)
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Seconds')?></td>
          <td><?php echo pql_format_error_span($error["pbstimeout"]); ?><input type="text" name="pbstimeout" value="<?php echo $_REQUEST["pbstimeout"]?>"></td>
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

// {{{ pbstimeout_save(type, host)
function pbstimeout_save($type, $host)  {
	global $_pql_control, $error, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = $_pql_control->get_dn($_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($host,
									 pql_get_define("PQL_ATTR_PBS_TIMEOUT"), '', $_REQUEST["pbstimeout"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_PBS_TIMEOUT"), 1, $_REQUEST["pbstimeout"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('pbstimeout options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('pbstimeout options')))
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


// {{{ pbstimeout_help(void)
function pbstimeout_help() {
	global $LANG;
?>
    Timeout in seconds until entries in the cache are invalidated.
    Only used by the pbsdbd server.
<?php
}
// }}}

// {{{ pbstimeout_help_cr(void)
function pbstimeout_help_cr() {
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
