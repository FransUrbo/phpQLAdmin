<?php
// control attribute plugin for
// idhost (host name for message id's)
//
// $Id: attrib.control.idhost.inc,v 2.25 2007-02-15 12:07:21 turbo Exp $

// {{{ idhost_check(type)
function idhost_check($type) {
	global $error, $LANG;

	// validity
	if(($_REQUEST["idhost"] != "") and !pql_check_hostaddress($_REQUEST["idhost"])) {
		$error["idhost"] = $LANG->_('Invalid');
		return false;
	}

	return true;
}
// }}}

// {{{ idhost_init(host)
// init values
function idhost_init($host) {
	global $_pql_control;
	
	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["idhost"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
											  pql_get_define("PQL_ATTR_IDHOST"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["idhost"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_IDHOST"));
	
	if(@is_null($_REQUEST["idhost"]))
	  return true;
}
// }}}


// {{{ idhost_print_view(host)
function idhost_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	idhost_init($host);
	$cn = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_CN"));
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">idhost (<?php echo $LANG->_('Host name for Message-ID\'s')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Host')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["idhost"]) { echo pql_maybe_idna_decode($_REQUEST["idhost"]); } else { echo $LANG->_('Default').': '.pql_maybe_idna_decode($cn)." (".$LANG->_('Taken from \'me\'') . ")"; }?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_IDHOST")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options').' '.pql_get_define("PQL_ATTR_IDHOST")?>"></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ idhost_print_form(void)
function idhost_print_form() {
	global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">idhost (<?php echo $LANG->_('Host name for Message-ID\'s')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Host')?></td>
          <td><?php echo pql_format_error_span($error["idhost"]); ?><input type="text" name="idhost" value="<?php echo $_REQUEST["idhost"]?>"> <?php echo $LANG->_('Default')?>: <?php echo $_REQUEST["mxhost"]?> (<?php echo $LANG->_('Taken from \'me\'')?>)</td>
        </tr>

        <tr>
          <td colspan="2" class="subtitle"><img src="images/info.png" width="16" height="16" border="0"><?php echo $LANG->_('Leave the fields empty to use default configuration')?>.</td>
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

// {{{ idhost_save(type, host)
function idhost_save($type, $host) {
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
									 pql_get_define("PQL_ATTR_IDHOST"), '', $_REQUEST["idhost"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_IDHOST"), '', $_REQUEST["idhost"]))
			$success = 0;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('idhost options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('idhost options')))
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


// {{{ idhost_help(void)
function idhost_help() {
	global $LANG;
?>
	Current host name, for use solely in saying hello to
	the remote SMTP server.  Default:  me, if that is
	supplied; otherwise qmail-remote refuses to run.
<?php
}
// }}}

// {{{ idhost_help_cr(void)
function idhost_help_cr() {
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
