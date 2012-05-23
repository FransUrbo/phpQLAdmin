<?php
// control attribute plugin for
// badmailfrom (blocked from addresses)
//
// $Id: attrib.control.badmailfrom.inc,v 2.28 2007-02-15 12:07:17 turbo Exp $

// {{{ badmailfrom_check(type)

function badmailfrom_check($type) {
    global $error, $LANG;
    
    // check validity
    if(@is_array($_REQUEST["badmailfrom"])) {
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

// {{{ badmailfrom_init(host)
// init values
function badmailfrom_init($host) {
    global $_pql_control;
    
    // fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["badmailfrom"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
												   pql_get_define("PQL_ATTR_BADMAILFROM"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["badmailfrom"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_BADMAILFROM"));
    
    if(!@$_REQUEST["badmailfrom"])
	  return true;
	elseif(!@is_array($_REQUEST["badmailfrom"]))
	  $_REQUEST["badmailfrom"] = array($_REQUEST["badmailfrom"]);
}

// }}}


// {{{ badmailfrom_print_view(host)
function badmailfrom_print_view($host) {
    global $_pql_control, $LANG;
    
    // init data
    badmailfrom_init($host);
    
    if(@is_array($_REQUEST["badmailfrom"])) {
		foreach($_REQUEST["badmailfrom"] as $key => $mail) {
			if(preg_match("/^@/", $mail))
			  $_REQUEST["badmailfrom"][$key] = "all of " . pql_strip_username($mail);
		}
    }
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">badmailfrom
      <tr>
        <td class="title" colspan="3"><?php echo $LANG->_('Blocked senders')?></td>
      </tr>

<?php
	$class   = pql_format_table(0);
	$url_tmp = "control_edit_attribute.php?mxhost=$host&attrib=".pql_get_define("PQL_ATTR_BADMAILFROM");

	$tbl_txt = array('tit' => $LANG->_('Email'), 'add' => $LANG->_('Add address'), 'del' => $LANG->_('Remove address'));
	$tbl_url = array('add' => $url_tmp.'&type=add', 'del' => $url_tmp.'&type=del&submit=1');
	pql_format_table_single( $tbl_txt, $tbl_url, $_REQUEST["badmailfrom"],
							pql_get_define("PQL_ATTR_BADMAILFROM"), $class);
?>
    </th>
  </table>
<?php
}
// }}}

// {{{ badmailfrom_print_form(void)
function badmailfrom_print_form() {
    global $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
<?php for($i=0; $i < count($_REQUEST["badmailfrom"]); $i++) { ?>
    <input type="hidden" name="badmailfrom[<?php echo $i?>]" value="<?php echo $_REQUEST["badmailfrom"][$i]?>">
<?php } ?>

    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">badmailfrom (<?php echo $LANG->_('Blocked senders')?>)
<?php $class = pql_format_table(0); ?>
        <tr>
          <td class="title"><?php echo $LANG->_('Email')?></td>
          <td class="<?php echo $class?>">
            <?php echo pql_format_error_span($error["badmailfrom"][$i]); ?>
            <input type="text" name="badmailfrom[<?php echo $i?>]" size="40">
          </td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?php echo $LANG->_('To block a whole domain simply leave user empty (eg. @domain.com). To delete a blocked address remove the value and save')?>.</td>
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

// {{{ badmailfrom_save(type, host)
function badmailfrom_save($type, $host) {
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
									 pql_get_define("PQL_ATTR_BADMAILFROM"), '', $_REQUEST["badmailfrom"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute(
								  pql_get_define("PQL_ATTR_CN")."=$host,". $_SESSION["USER_SEARCH_DN_CTR"],
								  pql_get_define("PQL_ATTR_BADMAILFROM"), '', $_REQUEST["badmailfrom"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('badmailfrom options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('badmailfrom options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
		// }}}

	  case "del":
		// {{{ delete entries
		if(pql_modify_attribute($host,
								pql_get_define("PQL_ATTR_BADMAILFROM"), $_REQUEST["badmailfrom"], ''))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('Bad mail from options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Bad mail from options')))
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
