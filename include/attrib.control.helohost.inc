<?php
// control attribute plugin for
// helohost (host name used in SMTP HELO command)
//
// $Id: attrib.control.helohost.inc,v 2.24 2006-12-16 12:02:14 turbo Exp $

// {{{ helohost_check(type)
function helohost_check($type) {
	global $error, $LANG;

	// validity
	if(($_REQUEST["helohost"] != "") and !pql_check_hostaddress($_REQUEST["helohost"])) {
		$error["helohost"] = $LANG->_('Invalid');
		return false;
	}
	
	return true;
}
// }}}

// {{{ helohost_init(host)
// init values
function helohost_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	if($host == "Global")
	  $value = pql_get_attribute($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
								 pql_get_define("PQL_ATTR_HELOHOST"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $value = pql_get_attribute($_pql_control->ldap_linkid, $host, pql_get_define("PQL_ATTR_HELOHOST"));

	if(@is_array($value))
	  $_REQUEST["helohost"] = $value[0];
	elseif($value)
	  $_REQUEST["helohost"] = $value;
}
// }}}


// {{{ helohost_print_view(host)
function helohost_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	helohost_init($host);
	$cn = pql_get_attribute($_pql_control->ldap_linkid, $host, pql_get_define("PQL_ATTR_CN"));
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">helohost (Helo host)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Host')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["helohost"]) { echo pql_maybe_idna_decode($_REQUEST["helohost"]); } else { echo $LANG->_('Default').': '.pql_maybe_idna_decode($cn)." (".$LANG->_('Taken from \'me\'') . ")"; }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_HELOHOST")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_HELOHOST")?>"></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ helohost_print_form(void)
function helohost_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">helohost (Helo host)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Host')?></td>
          <td><?php echo pql_format_error_span($error["helohost"]); ?><input type="text" name="helohost" value="<?=$_REQUEST["helohost"]?>"> <?=$LANG->_('Default')?>: <?=$mxhost?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

        <tr>
          <td colspan="2" class="subtitle"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Leave the fields empty to use default configuration')?>.</td>
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

// {{{ helohost_save(type, host)
function helohost_save($type, $host) {
	global $_pql_control, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = pql_get_dn($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($_pql_control->ldap_linkid, $host,
									 pql_get_define("PQL_ATTR_HELOHOST"), '', $_REQUEST["helohost"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($_pql_control->ldap_linkid, $host,
								  pql_get_define("PQL_ATTR_HELOHOST"), 1, $_REQUEST["helohost"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('helohost options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('helohost options')))
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


// {{{ helohost_help(void)
function helohost_help() {
	global $LANG;
?>
	Current host name, for use solely in saying hello to
	the remote SMTP server.  Default:  me, if that is
	supplied; otherwise qmail-remote refuses to run.
<?php
}
// }}}

// {{{ helohost_help_cr(void)
function helohost_help_cr() {
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
