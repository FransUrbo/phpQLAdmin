<?php
// control attribute plugin for
// idhost (host name for message id's)
//
// $Id: attrib.control.idhost.inc,v 2.9 2003-08-15 08:06:17 turbo Exp $

function idhost_check($type) {
	global $error, $idhost, $view, $LANG;

	// validity
	if($idhost != "" and !check_hostaddress($idhost)) {
		$error["idhost"] = $LANG->_('Invalid');
		return false;
	}

	return true;
}


// init values
function idhost_init($host) {
	global $_pql_control, $idhost, $view;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_GLOB_ATTR_IDHOST"));
	
	if(!is_null($value))
	  $idhost = $value[0];
}


function idhost_print_view($host) {
	global $_pql_control, $idhost, $view, $LANG;

	// init data
	idhost_init($host);

	// set empty field to default value
	if($idhost == "")
	  $idhost = $host . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")";
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">idhost (<?=$LANG->_('Host name for Message-ID\'s')?>)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$LANG->_('Host')?></td>
        <td><?$idhost?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=idhost"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function idhost_print_form() {
	global $attrib, $idhost, $PHP_SELF, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$PHP_SELF=?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">idhost (<?=$LANG->_('Host name for Message-ID\'s')?>)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Host')?></td>
          <td><?php echo format_error($error["idhost"]); ?><input type="text" name="idhost" value="<?=$idhost?>"> <?=$LANG->_('Default')?>: <?=$mxhost?> (<?=$LANG->_('Taken from \'me\'')?>)</td>
        </tr>

        <tr>
          <td colspan="2" class="subtitle"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Leave the fields empty to use default configuration')?>.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="mxhost" value="<?=$mxhost?>">
    <input type="hidden" name="view" value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function idhost_save($type, $host) {
	global $_pql_control, $attrib, $idhost, $error, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_GLOB_ATTR_IDHOST"),
										 $idhost))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('idhost options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('idhost options')))
			. ": " . pql_ldap_error($_pql_control->ldap_linkid);

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function idhost_help() {
	global $LANG;
?>
	Current host name, for use solely in saying hello to
	the remote SMTP server.  Default:  me, if that is
	supplied; otherwise qmail-remote refuses to run.
<?php
}

function idhost_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
