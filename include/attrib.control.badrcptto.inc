<?php
// control attribute plugin for
// badrcptto (blocked local addresses)
//
// $Id: attrib.control.badrcptto.inc,v 2.14.2.1 2003-12-09 16:18:24 dlw Exp $

function badrcptto_check($type) {
	global $error, $badrcptto, $view, $LANG;

	// check validity
	if(is_array($badrcptto)) {
		foreach($badrcptto as $key => $badmail) {
		 	if($badmail == "")
			  continue;
			
			if(!pql_check_email($badmail)) {
				$is_error = true;
				$error["badrcptto_$key"] = $LANG->_('Invalid');
			}
		} // end foreach
	} // end if is_array

	if($is_error)
	  return false;
	
	return true;
}

// init values
function badrcptto_init($host) {
	global $_pql_control, $badrcptto, $view;

	// fetch data from ldap server
	$badrcptto = pql_control_get_attribute($_pql_control->ldap_linkid,
										   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										   pql_get_define("PQL_GLOB_ATTR_BADRCPTTO"));

	if(is_null($badrcptto))
	  return true;
}

function badrcptto_print_view($host) {
	global $_pql_control, $badrcptto, $view, $LANG;

	// init data
	badrcptto_init($host);

	if(is_array($badrcptto)) {
		foreach($badrcptto as $key => $mail) {
			if(preg_match("/^@/", $mail))
			  $badrcptto[$key] = "all of " . pql_strip_username($mail);
		}
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">badrcptto (<?=$LANG->_('Rejected local addresses')?>)</th>
      <tr class="title">
        <td><?=$LANG->_('Email')?></td>
      </tr>

<?php
	if(!is_array($badrcptto)) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Not set')?></td>
      </tr>
<?php
	} else {
		foreach($badrcptto as $mail) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$mail?></td>
      </tr>

<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=badrcptto"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function badrcptto_print_form() {
	global $attrib, $PHP_SELF, $error, $badrcptto, $badcount, $mxhost, $view, $LANG;

	if(($badcount < count($badrcptto) - 1) and count($badrcptto) != 0)
	  $badcount = count($badrcptto) - 1;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">badrcptto (<?=$LANG->_('Rejected local addresses')?>)</th>
        <tr class="title">
          <td><?=$LANG->_('Address')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["badrcptto_0"]); ?><input type="text" name="badrcptto[0]" value="<?=$badrcptto[0]?>"></td>
        </tr>

<?php for($i = 1; $i <= $badcount; $i++) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["badrcptto_$i"]); ?><input type="text" name="badrcptto[<?=$i?>]" value="<?=$badrcptto[$i]?>"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td><a href="<?=$PHP_SELF?>?attrib=<?=$attrib?>&badcount=<?=($badcount+1)?>&mxhost=<?=$mxhost?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional address'))); ?></a> (<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
        </tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('To delete an address remove the value and save')?>.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="mxhost" value="<?=$mxhost?>">
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function badrcptto_save($type, $host) {
	global $_pql_control, $attrib, $badrcptto, $error, $mxhost, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $mxhost . "," . $_SESSION["USER_SEARCH_DN_CTR"], "badrcptto",
										 $badrcptto))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('badrcptto options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('badrcptto options')))
			. ": " . pql_ldap_error($_pql_control->ldap_linkid);

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function badrcptto_help() {
	global $LANG;
?>
Contains a list of local recipient addresses that are rejected. If the sender has RELAYCLIENT="" set this file has no effect.
<?php
}

function badrcptto_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from life with qmail-ldap');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
