<?php
// control attribute plugin for
// badmailfrom (blocked from addresses)
//
// $Id: attrib.control.badmailfrom.inc,v 2.10 2003-07-02 07:59:00 turbo Exp $

function badmailfrom_check($type) {
    global $error, $badmailfrom, $view, $LANG;
    
    // check validity
    if(is_array($badmailfrom)) {
		foreach($badmailfrom as $key => $badmail) {
			if($badmail == "")
			  continue;
	    
			if(preg_match("/^@/", $badmail)) {
				// retry with added user
				if(!check_email("user" . $badmail)) {
					$is_error = true;
					$error["badmailfrom_$key"] = $LANG->_('Invalid');
				}
			} else {
				if(!check_email($badmail)) {
					$is_error = true;
					$error["badmailfrom_$key"] = $LANG->_('Invalid');
				}
			} // end if preg_match...
		} // end foreach
    } // end if is_array
    
    if($is_error)
	  return false;
    
    return true;
}

// init values
function badmailfrom_init($host) {
    global $_pql_control, $badmailfrom, $view;
    
    // fetch data from ldap server
    $badmailfrom = pql_control_get_attribute($_pql_control->ldap_linkid,
					     "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
					     "badmailfrom");
    
    if(is_null($badmailfrom))
	  return true;
}

function badmailfrom_print_view($host) {
    global $_pql_control, $badmailfrom, $view, $LANG;
    
    // init data
    badmailfrom_init($host);
    
    if(is_array($badmailfrom)) {
		foreach($badmailfrom as $key => $mail) {
			if(preg_match("/^@/", $mail)) {
				$mail = split("@", $mail);
				$badmailfrom[$key] = "all of " . $mail[1];
			}
		}
    }
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">badmailfrom (<?=$LANG->_('Blocked senders')?>)</th>
      <tr class="title">
        <td><?=$LANG->_('Email')?></td>
      </tr>
 <?php
	if(!is_array($badmailfrom)) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=$LANG->_('Not set')?></td>
      </tr>
<?php
	} else {
		foreach($badmailfrom as $mail) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=$mail?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=badmailfrom"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function badmailfrom_print_form() {
    global $attrib, $PHP_SELF, $error, $badmailfrom, $badcount, $mxhost, $view, $LANG;
    
    if(($badcount < count($badmailfrom) - 1) and count($badmailfrom) != 0)
	  $badcount = count($badmailfrom) - 1;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">badmailfrom (<?=$LANG->_('Blocked senders')?>)</th>
        <tr class="title">
          <td><?=$LANG->_('Email')?></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td><?php echo format_error($error["badmailfrom_0"]); ?><input type="text" name="badmailfrom[0]" value="<?=$badmailfrom[0]?>"></td>
        </tr>

<?php for($i = 1; $i <= $badcount; $i++) { ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td><?php echo format_error($error["badmailfrom_$i"]); ?><input type="text" name="badmailfrom[<?=$i?>]" value="<?=$badmailfrom[$i]?>"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td><a href="<?=$PHP_SELF?>?attrib=<?=$attrib?>&badcount=<?($badcount+1)?>&mxhost=<?=$mxhost?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional address'))); ?></a> (<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
        </tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('To block a whole domain simply leave user empty (eg. @domain.com). To delete a blocked address remove the value and save')?>.</td>
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

function badmailfrom_save($type, $host) {
    global $_pql_control, $attrib, $badmailfrom, $error, $view, $LANG;
    
    switch($type) {
      case "modify";
      case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 "badmailfrom", $badmailfrom))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('badmailfrom options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('badmailfrom options')))
			. ": " . pql_ldap_error($_pql_control->ldap_linkid);

		attribute_forward($msg);
		break;

      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}

function badmailfrom_help() {
	global $LANG;
?>
	Unacceptable envelope sender addresses.  qmail-smtpd
	will reject every recipient address for a message if
	the envelope sender address is listed in badmailfrom.
	A line in badmailfrom may be of the form @host, meaning
	every address at host.
<?php
}

function badmailfrom_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
