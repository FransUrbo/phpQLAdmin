<?php
// control attribute plugin for
// badmailfrom (blocked from addresses)
//
// $Id: attrib.control.badmailfrom.inc,v 2.19 2004-10-19 10:40:40 turbo Exp $

// {{{ badmailfrom_check(type)

function badmailfrom_check($type) {
    global $error, $LANG;
    
    // check validity
    if(is_array($_REQUEST["badmailfrom"])) {
		foreach($_REQUEST["badmailfrom"] as $key => $badmail) {
			if($badmail == "")
			  continue;
	    
			if(preg_match("/^@/", $badmail)) {
				// retry with added user
				if(!pql_check_email("user" . $badmail)) {
					$is_error = true;
					$error["badmailfrom_$key"] = $LANG->_('Invalid');
				}
			} else {
				if(!pql_check_email($badmail)) {
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

// }}}

// {{{ badmailfrom_init($host)

// init values
function badmailfrom_init($host) {
    global $_pql_control;
    
    // fetch data from ldap server
    $_REQUEST["badmailfrom"] = pql_get_attribute($_pql_control->ldap_linkid,
												 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
												 pql_get_define("PQL_ATTR_BADMAILFROM"));
    
    if(is_null($_REQUEST["badmailfrom"]))
	  return true;
}

// }}}

// {{{ badmailfrom_print_view(host)
function badmailfrom_print_view($host) {
    global $_pql_control, $LANG;
    
    // init data
    badmailfrom_init($host);
    
    if(is_array($_REQUEST["badmailfrom"])) {
		foreach($_REQUEST["badmailfrom"] as $key => $mail) {
			if(preg_match("/^@/", $mail))
			  $_REQUEST["badmailfrom"][$key] = "all of " . pql_strip_username($mail);
		}
    }
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">badmailfrom (<?=$LANG->_('Blocked senders')?>)</th>
      <tr class="title">
        <td><?=$LANG->_('Email')?></td>
      </tr>
 <?php
	if(!is_array($_REQUEST["badmailfrom"])) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=$LANG->_('Not set')?></td>
      </tr>
<?php
	} else {
		foreach($_REQUEST["badmailfrom"] as $mail) {
?>
      <tr class="<?php pql_format_table(); ?>">
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
// }}}

// {{{ badmailfrom_print_form(void)
function badmailfrom_print_form() {
    global $error, $LANG;
    
    if(($_REQUEST["badcount"] < count($_REQUEST["badmailfrom"]) - 1) and count($_REQUEST["badmailfrom"]) != 0)
	  $_REQUEST["badcount"] = count($_REQUEST["badmailfrom"]) - 1;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">badmailfrom (<?=$LANG->_('Blocked senders')?>)</th>
        <tr class="title">
          <td><?=$LANG->_('Email')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["badmailfrom_0"]); ?><input type="text" name="badmailfrom[0]" value="<?=$_REQUEST["badmailfrom"][0]?>"></td>
        </tr>

<?php for($i = 1; $i <= $_REQUEST["badcount"]; $i++) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["badmailfrom_$i"]); ?><input type="text" name="badmailfrom[<?=$i?>]" value="<?=$_REQUEST["badmailfrom"][$i]?>"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td><a href="<?=$_SERVER["PHP_SELF"]?>?attrib=<?=$_REQUEST["attrib"]?>&badcount=<?($_REQUEST["badcount"]+1)?>&mxhost=<?=$_REQUEST["mxhost"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional address'))); ?></a> (<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
        </tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('To block a whole domain simply leave user empty (eg. @domain.com). To delete a blocked address remove the value and save')?>.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?=$_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ badmailfrom_save(type, host)
function badmailfrom_save($type, $host) {
    global $_pql_control, $LANG;
    
    switch($type) {
      case "modify";
      case "add";
		// save entries
		if(pql_modify_attribute($_pql_control->ldap_linkid,
								pql_get_define("PQL_ATTR_CN")."=$host,". $_SESSION["USER_SEARCH_DN_CTR"],
								pql_get_define("PQL_ATTR_BADMAILFROM"), '', $_REQUEST["badmailfrom"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('badmailfrom options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('badmailfrom options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;

      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}
// }}}


// {{{ badmailfrom_help(void)
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
// }}}

// {{{ badmailfrom_help_cr(void)
function badmailfrom_help_cr() {
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
