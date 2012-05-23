<?php
// control attribute plugin for
// localiphost (name substituted for local IP address)
//
// $Id: attrib.control.localiphost.inc,v 2.24 2007-02-15 12:07:34 turbo Exp $

// {{{ ocaliphost_check(type)
function localiphost_check($type) {
	global $error, $LANG;

	// validity
	if(($_REQUEST["localiphost"] != "") and !pql_check_hostaddress($_REQUEST["localiphost"])) {
		$error["localiphost"] = $LANG->_('Invalid');
		return false;
	}
	
	return true;
}
// }}}

// {{{ localiphost_init(host)
// init values
function localiphost_init($host) {
	global $_pql_control;
	
	// fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["localiphost"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
												   pql_get_define("PQL_ATTR_LOCALIPHOST"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["localiphost"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_LOCALIPHOST"));
	
	if(@is_null($_REQUEST["localiphost"]))
	  return true;
}
// }}}


// {{{ localiphost_print_view(host)
function localiphost_print_view($host) {
	global $_pql_control, $LANG;
	
	// init data
	localiphost_init($host);
	$cn = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_CN"));
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">localiphost (<?php echo $LANG->_('Substitution for local IP')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Host')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["localiphost"]) { echo pql_maybe_idna_decode($_REQUEST["localiphost"]); } else { echo $LANG->_('Default').': '.pql_maybe_idna_decode($cn)." (" . $LANG->_('Taken from \'me\'') . ")"; }?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_LOCALIPHOST")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_LOCALIPHOST")?>"></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ localiphost_print_form(void)
function localiphost_print_form() {
	global $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">localiphost (<?php echo $LANG->_('Substitution for local IP')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Host')?></td>
          <td><?php echo pql_format_error_span($error["localiphost"]); ?><input type="text" name="localiphost" value="<?php echo $localiphost; ?>"> <?php echo $LANG->_('Default')?>: <?=$_REQUEST["mxhost"]?> (<?=$LANG->_('Value in \'me\'')?>)</td>
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

// {{{ localiphost_save(type, host)
function localiphost_save($type, $host) {
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
									 pql_get_define("PQL_ATTR_LOCALIPHOST"), '', $_REQUEST["localiphost"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_LOCALIPHOST"), 1, $_REQUEST["localiphost"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('localiphost options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('localiphost options')))
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


// {{{ localiphost_help(void)
function localiphost_help() {
	global $LANG;
?>
	Replacement host name for local IP addresses.  Default:
	me, if that is supplied.  qmail-smtpd is responsible
	for recognizing dotted-decimal addresses for the
	current host.  When it sees a recipient address of the
	form box@[d.d.d.d], where d.d.d.d is a local IP
	address, it replaces [d.d.d.d] with localiphost.  This
	is done before rcpthosts.
<?php
}
// }}}

// {{{ localiphost_help_cr(void)
function localiphost_help_cr() {
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
