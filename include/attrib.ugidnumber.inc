<?php
// attribute plugin for
// qmail uid and gid
// $Id: attrib.ugidnumber.inc,v 2.10 2007-02-15 12:08:09 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type) {
    global $error, $LANG;

	$failed = false;

	// Just to save some typing.
	$uidnumber = (!empty($_REQUEST["uidnumber"]) ? trim($_REQUEST["uidnumber"]) : "");
	$gidnumber = (!empty($_REQUEST["gidnumber"]) ? trim($_REQUEST["gidnumber"]) : "");
	
	if ($uidnumber == "") {
		$error["uidnumber"] = $LANG->_('Missing');
		$failed = true;
	} else if (preg_match('/\D/', $uidnumber)) {
		$error["uidnumber"] = $LANG->_('Must be a number');
		$failed = true;
	}
	
	if ($gidnumber == "") {
		$error["gidnumber"] = $LANG->_('Missing');
		$failed = true;
	} else if (preg_match('/\D/', $gidnumber)) {
		$error["gidnumber"] = $LANG->_('Must be a number');
		$failed = true;
	}
	
	return !$failed;
}
// }}}

// {{{ parse_oldvalue(void)
function parse_oldvalue() {
	$uid_gid = (empty($_REQUEST["oldvalue"]) ? ";" : $_REQUEST["oldvalue"]);
	
	// This should always work because the value comes from the database.
	if (preg_match('/^\s*(\d*)\s*;\s*(\d*)\s*$/', $uid_gid, $results))
	  return array($results[1], $results[2]);
	else
	  // but just to be safe.
	  return array($uid_gid, '');
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    global $_pql;

	// uidNumber and gidNumber are 2 LDAP fields that are passed around
	// as a string in the format: uidNumber;gidNumber
	$uid_gid = (empty($_PEQUEST["oldvalue"]) ? ";" : $_REQUEST["oldvalue"]);

    list($_REQUEST["uidnumber"], $_REQUEST["gidnumber"]) = parse_oldvalue();

	if (empty($_REQUEST["uidnumber"])) {
	  $_REQUEST["uidnumber"] = pql_get_next_ugidnumber('uid', $_REQUEST["rootdn"]);
	}
	if (empty($_REQUEST["gidnumber"])) {
	  $_REQUEST["gidnumber"] = pql_get_next_ugidnumber('gid', $_REQUEST["rootdn"]);
	}
}
// }}}

// {{{ attribute_print_form(type)
function attribute_print_form($type = "modify") {
    global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Change email UID/GID')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('UID')?></td>
          <td><?php echo pql_format_error_span($error["uidnumber"]); ?><input
				  type="text" name="uidnumber" value="<?php echo $_REQUEST["uidnumber"]; ?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('GID')?></td>
          <td><?php echo pql_format_error_span($error["gidnumber"]); ?><input
				  type="text" name="gidnumber" value="<?php echo $_REQUEST["gidnumber"]; ?>"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="domain" value="<?php echo urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn" value="<?php echo urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="oldvalue" value="<?php echo $_REQUEST["oldvalue"]?>">
    <input type="hidden" name="user"     value="<?php echo urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view" value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>

<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;

	switch($type) {
	  case "add":
		break;
	  case "modify":
	    // We have 2 values to change which complicates this a bit.
		list($old_uidnumber, $old_gidnumber) = parse_oldvalue();
		$msgs = array();

	    //die($_REQUEST["user"]);
		if ($_REQUEST["uidnumber"] != $old_uidnumber) {
			if (pql_modify_attribute($_REQUEST["user"], "uidnumber", 1, $_REQUEST["uidnumber"]))
			  $msgs[] = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
											  array('what' => $LANG->_('UID', 4),
													'new'  => $_REQUEST["uidnumber"]));
			else
			  $msgs[] = pql_complete_constant($LANG->_('Failed to change %what%'),
											  array('what' => $LANG->_('UID', 4))) . ": " . ldap_error($_pql->ldap_linkid);
		}

		if ($_REQUEST["gidnumber"] != $old_gidnumber) {
			if (pql_modify_attribute($_REQUEST["user"], "gidnumber", 1, $_REQUEST["gidnumber"]))
			  $msgs[] = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
											  array('what' => $LANG->_('GID', 4),
													'new'  => $_REQUEST["gidnumber"]));
			else
			  $msgs[] = pql_complete_constant($LANG->_('Failed to change %what%'),
											  array('what' => $LANG->_('GID', 4))) . ": " . ldap_error($_pql->ldap_linkid);
		}
		
		if (!empty($msgs)) {
		  $msg = implode(" ", $msgs);
    	} else {
		  $msg = pql_complete_constant($LANG->_('%uwhat% and %gwhat% unchanged'),
									   array('uwhat' => $LANG->_('UID'), 'gwhat' => $LANG->_('GID')));
		}
		
		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
