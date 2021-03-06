<?php
// control attribute plugin for
// smtpgreeting (SMTP greeting message)
//
// $Id: attrib.control.smtpgreeting.inc,v 2.24 2007-02-15 12:08:05 turbo Exp $

// {{{ smtpgreeting_check(type)
function smtpgreeting_check($type) {
    global $error;
    
    // no check necessary
    return true;
}
// }}}

// {{{ smtpgreeting_init(host)
// init values
function smtpgreeting_init($host) {
    global $_pql_control;
    
    // fetch data from ldap server
	if($host == "Global")
	  $_REQUEST["smtpgreeting"] = $_pql_control->get_attribute($_SESSION["USER_SEARCH_DN_CTR"],
													pql_get_define("PQL_ATTR_SMTPGREETING"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $_REQUEST["smtpgreeting"] = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_SMTPGREETING"));
    
    if(@is_null($_REQUEST["smtpgreeting"]))
	  return true;
}
// }}}


// {{{ smtpgreeting_print_view(host)
function smtpgreeting_print_view($host) {
    global $_pql_control, $LANG;
    
    // init data
    smtpgreeting_init($host);
	$cn = $_pql_control->get_attribute($host, pql_get_define("PQL_ATTR_CN"));
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">smtpgreeting (<?php echo $LANG->_('SMTP greeting message')?>)</th>
<?php $class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?php echo $LANG->_('Message')?></td>
        <td class="<?php echo $class?>"><?php if(@$_REQUEST["smtpgreeting"]) { echo pql_maybe_idna_decode($_REQUEST["smtpgreeting"]); } else { echo $LANG->_('Default').': '.pql_maybe_idna_decode($cn)." (".$LANG->_('Taken from \'me\'') . ")"; }?></td>
        <td class="<?php echo $class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?php echo $host?>&attrib=<?php echo pql_get_define("PQL_ATTR_SMTPGREETING")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change options').' '.pql_get_define("PQL_ATTR_SMTPGREETING")?>"></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ smtpgreeting_print_form(void)
function smtpgreeting_print_form() {
    global $attrib, $smtpgreeting, $error, $mxhost, $view, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">smtpgreeting (<?php echo $LANG->_('SMTP greeting message')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Message')?></td>
          <td><?php echo pql_format_error_span($error["smtpgreeting"]); ?><input type="text" name="smtpgreeting" value="<?php echo $_REQUEST["smtpgreeting"]?>"> <?php echo $LANG->_('Default')?>: <?php echo $mxhost?> (<?php echo $LANG->_('Value in \'me\'', 4)?>)</td>
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

// {{{ smtpgreeting_save(type, host)
function smtpgreeting_save($type, $host) {
    global $_pql_control, $error, $LANG;
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
									 pql_get_define("PQL_ATTR_SMTPGREETING"), '', $_REQUEST["smtpgreeting"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($host,
								  pql_get_define("PQL_ATTR_SMTPGREETING"), 1, $_REQUEST["smtpgreeting"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('smtpgreeting options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('smtpgreeting options'))) . ": " . pql_format_error(0);
																							   
		attribute_forward($msg);
		break;
		// }}}

      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}
// }}}


// {{{ smtpgreeting_help(void)
function smtpgreeting_help() {
    global $LANG;
?>
  SMTP greeting message.  Default:  me, if that is
  supplied; otherwise qmail-smtpd will refuse to run.
  The first word of smtpgreeting should be the current
  host's name.
<?php
}
// }}}

// {{{ smtpgreeting_help_cr(void)
function smtpgreeting_help_cr() {
    global $LANG;
    echo $LANG->_('Help text taken from qmail man pages'), $LANG;
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
