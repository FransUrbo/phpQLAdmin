<?php
// control attribute plugin for
// custombouncetext (additional bounce message)
//
// $Id: attrib.control.custombouncetext.inc,v 2.12.2.1 2003-12-09 16:18:24 dlw Exp $

function custombouncetext_check($type) {
	global $error, $custombouncetext, $use_custombouncetext, $view, $LANG;

	// missing
	if($use_custombouncetext == 1 and $custombouncetext == "") {
		$error["custombouncetext"] = $LANG->_('Missing');
		return false;
	}

	return true;
}

// init values
function custombouncetext_init($host) {
	global $_pql_control, $custombouncetext, $use_custombouncetext, $view;
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_GLOB_ATTR_CUSTOMBOUNCETEXT"));
	
	if(!is_null($value))
	  $custombouncetext = $value[0];
	
	// initialize radio button
	if($custombouncetext == "")
	  $use_custombouncetext = 0;
	else
	  $use_custombouncetext = 1;
}

function custombouncetext_print_view($host) {
	global $_pql_control, $custombouncetext, $use_custombouncetext, $view, $LANG;
	
	// init data
	custombouncetext_init($host);
	
	if($use_custombouncetext == "0")
	  $custombouncetext = "disabled";
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">custombouncetext (<?=$LANG->_('Additional bounce text')?>) for host <u><?=$host?></u></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Message')?></td>
        <td><?php echo nl2br($custombouncetext); ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=custombouncetext"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function custombouncetext_print_form() {
	global $attrib, $custombouncetext, $use_custombouncetext,  $PHP_SELF, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">custombouncetext (<?=$LANG->_('Additional bounce text')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Use additional message')?></td>
          <td>
            <input type="radio" name="use_custombouncetext" value="0" <?php if($use_custombouncetext == "0"){ echo "checked"; } //" ?>> <?=$LANG->_('No')?>
            <input type="radio" name="use_custombouncetext" value="1" <?php if($use_custombouncetext == "1"){ echo "checked"; } //" ?>> <?=$LANG->_('Yes')?>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Message')?></td>
          <td><?php echo pql_format_error_span($error["custombouncetext"]); ?><textarea cols="40" rows="10" name="custombouncetext"><?=$custombouncetext?></textarea></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Additional custom text in bounce messages, eg. for providing contact information')?>.</td>
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

function custombouncetext_save($type, $host) {
	global $_pql_control, $attrib, $custombouncetext, $use_custombouncetext, $error, $mxhost, $view, $LANG;
	
	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if($use_custombouncetext == "0")
		  $custombouncetext = "";
		
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $mxhost . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_GLOB_ATTR_CUSTOMBOUNCETEXT"),
										 $custombouncetext)) 
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('custombouncetext message')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('custombouncetext message')))
			. ": " . pql_ldap_error($_pql_control->ldap_linkid);
		
		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
