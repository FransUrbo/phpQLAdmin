<?php
// control attribute plugin for
// envnoathost (default prefix for mailbox paths)
//
// $Id: attrib.control.envnoathost.inc,v 2.21 2006-05-19 06:55:31 turbo Exp $

// {{{ envnoathost_check(type)
function envnoathost_check($type) {
	global $error, $LANG;

	// missing
	if($_REQUEST["envnoathost"] == "") {
		$error["envnoathost"] = $LANG->_('Missing');
		return false;
	}

	// validity
	if(!pql_check_hostaddress($_REQUEST["envnoathost"])) {
		$error["envnoathost"] = $LANG->_('Invalid');
		return false;
	}

	return true;
}
// }}}

// {{{ envnoathost_init(host)
// init values
function envnoathost_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	if($host == "Global") {
	  $_REQUEST["envnoathost"] = pql_get_attribute($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
												   pql_get_define("PQL_ATTR_ENVNOATHOST"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	} else {
	  $_REQUEST["envnoathost"] = pql_get_attribute($_pql_control->ldap_linkid,
												   pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
												   pql_get_define("PQL_ATTR_ENVNOATHOST"));
	}

	if(@is_null($_REQUEST["envnoathost"]))
	  return true;
}
// }}}


// {{{ envnoathost_print_view(host)
function envnoathost_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	envnoathost_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">envnoathost (<?=$LANG->_('Default domain for addresses without @')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Email')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["envnoathost"]) { echo pql_maybe_idna_decode($_REQUEST["envnoathost"]); } else { echo $LANG->_('Default').': '.pql_maybe_idna_decode($host)." (" . $LANG->_('Taken from \'me\'') . ")"; }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_ENVNOATHOST")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options').' '.pql_get_define("PQL_ATTR_ENVNOATHOST")?>"></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ envnoathost_print_form(void)
function envnoathost_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">envnoathost (<?=$LANG->_('Default domain for addresses without @')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Email')?></td>
          <td><?php echo pql_format_error_span($error["envnoathost"]); ?><input type="text" name="envnoathost" value="<?=$_REQUEST["envnoathost"]?>"> <?=$LANG->_('Default')?>: <?=$mxhost?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Leave the fields empty to use default configuration')?>.</td>
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

// {{{ envnoathost_save(type, host)
function envnoathost_save($type, $host) {
	global $_pql_control, $LANG;
	$success = 0;

	switch($type) {
	  case "modify":
	  case "add":
		// {{{ save entries
		if($host == "Global") {
		  // Get all QLC host
		  $qlcs = pql_get_dn($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
							 '(&(cn=*)(objectclass=qmailControl))', 'ONELEVEL');
		  
		  $success = 1;
		  foreach($qlcs as $host) {
			if(!pql_modify_attribute($_pql_control->ldap_linkid, $host,
									 pql_get_define("PQL_ATTR_ENVNOATHOST"), '', $_REQUEST["envnoathost"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($_pql_control->ldap_linkid,
								  pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
								  pql_get_define("PQL_ATTR_ENVNOATHOST"), 1, $_REQUEST["envnoathost"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('envnotathost options')));
		else 
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('envnoathost options')))
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


// {{{ envnoathost_help(void)
function envnoathost_help() {
	global $LANG;
?>
	Presumed domain name for addresses without @ signs.
	Default:  me, if that is supplied; otherwise the
	literal name envnoathost, which is probably not what
	you want.  If qmail-send sees an envelope recipient
	address without an @ sign, it appends @envnoathost.
<?php
}
// }}}

// {{{ envnotathost_help_cr(void)
function envnotathost_help_cr() {
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
