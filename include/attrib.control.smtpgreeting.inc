<?php
// control attribute plugin for
// smtpgreeting (SMTP greeting message)
//
// $Id: attrib.control.smtpgreeting.inc,v 2.15 2004-03-11 16:31:55 turbo Exp $

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
    $value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_ATTR_SMTPGREETING"));
    
    if(!is_null($value))
      $_REQUEST["smtpgreeting"] = $value[0];
}
// }}}


// {{{ smtpgreeting_print_view(host)
function smtpgreeting_print_view($host) {
    global $_pql_control, $LANG;
    
    // init data
    smtpgreeting_init($host);
    
    // set empty field to default value
    if($_REQUEST["smtpgreeting"] == "")
      $_REQUEST["smtpgreeting"] = pql_maybe_idna_decode($host) . ", (" . $LANG->_('Default') . ", " . $LANG->_('Taken from \'me\'') . ")";
	else
	  $_REQUEST["smtpgreeting"] = pql_maybe_idna_decode($_REQUEST["smtpgreeting"]);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">smtpgreeting (<?=$LANG->_('SMTP greeting message')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Message')?></td>
        <td><?=$_REQUEST["smtpgreeting"]?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=smtpgreeting"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
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
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">smtpgreeting (<?=$LANG->_('SMTP greeting message')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Message')?></td>
          <td><?php echo pql_format_error_span($error["smtpgreeting"]); ?><input type="text" name="smtpgreeting" value="<?=$_REQUEST["smtpgreeting"]?>"> <?=$LANG->_('Default')?>: <?=$mxhost?> (<?=$LANG->_('Value in \'me\'', 4)?>)</td>
        </tr>

        <tr>
          <td colspan="2" class="subtitle"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Leave the fields empty to use default configuration')?>.</td>
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

// {{{ smtpgreeting_save(type, host)
function smtpgreeting_save($type, $host) {
    global $_pql_control, $error, $LANG;
    
    switch($type) {
      case "modify";
      case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_ATTR_CN") . "=" . $mxhost . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_ATTR_SMTPGREETING"),
										 $_REQUEST["smtpgreeting"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('smtpgreeting options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('smtpgreeting options'))) . ": " . pql_format_error(0);
																							   
		attribute_forward($msg);
		break;

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
