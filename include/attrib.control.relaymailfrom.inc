<?php
// control attribute plugin for
// relaymailfrom (allowed sender addresses for relaying)
//
// $Id: attrib.control.relaymailfrom.inc,v 2.13 2003-11-20 08:01:36 turbo Exp $

function relaymailfrom_check($type) {
	global $error, $relaymailfrom, $view, $LANG;
	
	// check validity
	if(is_array($relaymailfrom)) {
		foreach($relaymailfrom as $key => $badmail) {
		 	if($badmail == "")
			  continue;
			
			if(preg_match("/^@/", $badmail)) {
				// retry with added user
				if(!pql_check_email("user" . $badmail)) {
					$is_error = true;
					$error["relaymailfrom_$key"] = $LANG->_('Invalid');
				}
			} else {
				if(!pql_check_email($badmail)) {
					$is_error = true;
					$error["relaymailfrom_$key"] = $LANG->_('Invalid');
				}
			} // end if preg_match...
		} // end foreach
	} // end if is_array
	
	if($is_error)
	  return false;
	
	return true;
}

function relaymailfrom_init($host) {
	// init values
	global $_pql_control, $relaymailfrom, $view;
	
	// fetch data from ldap server
	$relaymailfrom = pql_control_get_attribute($_pql_control->ldap_linkid,
											   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
											   pql_get_define("PQL_GLOB_ATTR_RELAYMAILFROM"));
	
	if(is_null($relaymailfrom))
	  return true;
}

function relaymailfrom_print_view($host) {
	global $_pql_control, $relaymailfrom, $view, $LANG;

	// init data
	relaymailfrom_init($host);
	
	if(is_array($relaymailfrom)) {
		foreach($relaymailfrom as $key => $mail) {
			if(preg_match("/^@/", $mail))
			  $relaymailfrom[$key] = "all of " . pql_strip_username($mail);
		}
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">relaymailfrom (<?=$LANG->_('Allowed senders for relaying', 4)?>)</th>
      <tr class="title">
        <td><?=$LANG->_('Email')?></td>
      </tr>
<?php
	if(!is_array($relaymailfrom)) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('No address defined')?></td>
      </tr>
<?php
	} else {
		foreach($relaymailfrom as $mail) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$mail?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=relaymailfrom"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function relaymailfrom_print_form() {
	global $attrib, $PHP_SELF, $error, $relaymailfrom, $relaycount, $mxhost, $view, $LANG;
	
	if(($relaycount < count($relaymailfrom) - 1) and count($relaymailfrom) != 0)
	  $relaycount = count($relaymailfrom) - 1;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">relaymailfrom (<?=$LANG->_('Allowed senders for relaying', 4)?>)</th>
        <tr class="title">
          <td><?=$LANG->_('Address')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["relaymailfrom_0"]); ?><input type="text" name="relaymailfrom[0]" value="<?=$relaymailfrom[0]?>"></td>
        </tr>

<?php
  for($i = 1; $i <= $relaycount; $i++){
?>
        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["relaymailfrom_$i"]); ?><input type="text" name="relaymailfrom[<?=$i?>]" value="<?=$relaymailfrom[$i]?>"></td>
        </tr>

<?php	} ?>
        <tr class="subtitle">
          <td><a href="<?$PHP_SELF?>?attrib=<?$attrib?>&relaycount=<?=($relaycount+1)?>&mxhost=<?=$mxhost?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional address'))); ?></a> (<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
        </tr>

        <tr class="subtitle">
          <td>
            <img src="images/info.png" width="16" height="16" border="0">Note: this is a really bad idea, don't use this for a public server !!!
            <br> To allow relaying for a whole domain simply leave user empty (eg. @domain.com). To delete a relaying remove the value and save.
          </td>
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
// '
function relaymailfrom_save($type, $host) {
	global $_pql_control, $attrib, $relaymailfrom, $error, $view, $LANG;

	switch($type) {
		case "modify";
		case "add";
			// save entries
			if(pql_control_replace_attribute($_pql_control->ldap_linkid,
											 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
											 pql_get_define("PQL_GLOB_ATTR_RELAYMAILFROM"),
											 $relaymailfrom))
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										   array('what' => $LANG->_('relaymailfrom options')));
			else
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('relaymailfrom options'))) . ": " . pql_ldap_error($_pql_control->ldap_linkid);

			attribute_forward($msg);
		    break;

		default:
		  die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
									array('type' => $type, 'file' => __FILE__)));
	}
}

function relaymailfrom_help() {
	global $LANG;
?>
This file contains envelope sender addresses that are allowed to relay through this server. This is a really bad idea as sender addresses are very easy to spoof and you are an
open relay then. You should use SMTP after POP instead.
<br><br>
    joe@yourdomain.com<br>
    @otherdomain.com
<br><br>
The first example allows joe@yourdomain.com to relay, the second one allows all addresses ending with @otherdomain.com to relay.
<?php
}

function relaymailfrom_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from life with qmail-ldap');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
