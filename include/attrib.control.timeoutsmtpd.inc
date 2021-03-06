<?php
// control attribute plugin for
// timeoutsmtpd (how long to wait for smtp client)
//
// $Id: attrib.control.timeoutsmtpd.inc,v 2.23 2007-02-15 12:08:05 turbo Exp $

// {{{ timeoutsmtpd_check(type)
function timeoutsmtpd_check($type) {
	global $error, $LANG;

	// number ?
	if(preg_match ("/[^0-9]/", $_REQUEST["timeoutsmtpd"])) {
		$error["timeoutsmtpd"] = $LANG->_('Invalid');
		return false;
	}

	// must be > 0
	if(@empty($error["timeoutsmtpd"]) and ($_REQUEST["timeoutsmtpd"] <= 0)) {
		$error["timeoutsmtpd"] = $LANG->_('Must be above zero');
		return false;
	}

	return true;
}
// }}}

// {{{ timeoutsmtpd_init(host)
// init values
function timeoutsmtpd_init($host) {
	global $_pql_control, $timeoutsmtpd;
	
	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["timeoutsmtpd"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
													pql_get_define("PQL_ATTR_TIMEOUTSMTPD"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["timeoutsmtpd"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_TIMEOUTSMTPD"));
	
	if(@is_null($_REQUEST["qmqpcip"]))
	  return true;
}
// }}}


// {{{ timeoutsmtpd_print_view(host)
function timeoutsmtpd_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	timeoutsmtpd_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('Timeoutsmtpd (How long to wait for SMTP client)')?></th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Wait')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["timeoutsmtpd"]) { echo $_REQUEST["timeoutsmtpd"]; } else { echo $LANG->_('Default', 4).': 1200'; }?> <?php echo $LANG->_('Seconds', 4)?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_TIMEOUTSMTPD")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options', 4).' '.pql_get_define("PQL_ATTR_TIMEOUTSMTPD")?>"></a></td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ timeoutsmtpd_print_form(void)
function timeoutsmtpd_print_form() {
	global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Timeoutsmtpd (How long to wait for SMTP client)')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Wait')?></td>
          <td><?php echo pql_format_error_span($error["timeoutsmtpd"]); ?><input type="text" name="timeoutsmtpd" value="<?php echo $_REQUEST["timeoutsmtpd"]?>"><?php echo $LANG->_('Seconds', 4)?></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?php echo pql_complete_constant($LANG->_('Empty field to set to the default value (%default% seconds)'), array('default' => '1200')); ?></td>
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

// {{{ timeoutsmtpd_save(type, host)
function timeoutsmtpd_save($type, $host) {
	global $_pql_control, $LANG;
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
									 pql_get_define("PQL_ATTR_TIMEOUTSMTPD"), '', $_REQUEST["timeoutsmtpd"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_TIMEOUTSMTPD"), 1, $_REQUEST["timeoutsmtpd"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('timeoutsmtpd options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('timeoutsmtpd options'))) . ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
		// }}}

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ timeoutsmtpd_help(void)
function timeoutsmtpd_help() {
	global $LANG;
?>
	Number of seconds qmail-smtpd will wait for each new
	buffer of data from the remote SMTP client.  Default:
	1200.
<?php
}
// }}}

// {{{ timeoutsmtpd_help_cr(void)
function timeoutsmtpd_help_cr() {
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
