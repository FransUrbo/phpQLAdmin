<?php
// control attribute plugin for
// quotawarning (additional quota warning message)
//
// $Id: attrib.control.quotawarning.inc,v 2.13 2004-03-01 10:09:07 turbo Exp $

function quotawarning_check($type) {
	global $error, $quotawarning, $use_quotawarning, $view, $LANG;

	// missing
	if($use_quotawarning == 1 and $quotawarning == "") {
		$error["quotawarning"] = $LANG->_('Missing');
		return false;
	}

	return true;
}

function quotawarning_init($host) {
	// init values
	global $_pql_control, $quotawarning, $use_quotawarning, $view;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
									   pql_get_define("PQL_GLOB_ATTR_QUOTA_WARNING"));

	if(!is_null($value))
	  $quotawarning = $value[0];

	// initialize radio button
	if($quotawarning == "")
	  $use_quotawarning = 0;
	else
	  $use_quotawarning = 1;
}

function quotawarning_print_view($host) {
	global $_pql_control, $quotawarning, $use_quotawarning, $view, $LANG;
	
	// init data
	quotawarning_init($host);
	
	if($use_quotawarning == "0")
	  $quotawarning = "disabled";
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">quotawarning (<?=$LANG->_('Additional quota warning message')?>)</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Message')?></td>
        <td><?php echo nl2br($quotawarning); ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=quotawarning"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function quotawarning_print_form() {
	global $attrib, $quotawarning, $use_quotawarning, $error, $mxhost, $view, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">quotawarning (<?=$LANG->_('Additional quota warning message')?>)</th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Send quota warnings')?></td>
          <td>
            <input type="radio" name="use_quotawarning" value="0" <?php if($use_quotawarning == "0"){ echo "checked"; } //" ?>> no
            <input type="radio" name="use_quotawarning" value="1" <?php if($use_quotawarning == "1"){ echo "checked"; } //" ?>> yes
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Message')?></td>
          <td><?php echo pql_format_error_span($error["quotawarning"]); ?><textarea cols="40" rows="10" name="quotawarning"><?=$quotawarning?></textarea></td>
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

function quotawarning_save($type, $host) {
	global $_pql_control, $attrib, $quotawarning, $use_quotawarning, $error, $view, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if($use_quotawarning == "0")
		  $quotawarning = "";
		
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_GLOB_ATTR_QUOTA_WARNING"),
										 $quotawarning))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('quotawarning message')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('quotawarning message')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function quotawarning_help() {
	global $LANG;
	// life with qmail-ldap (http://www.lifewithqmail.org/ldap/)
?>
	Custom text in quota warning message. If you set to no, no quota warning
	messages will be issued
<?php
}

function quotawarning_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from life with qmail-ldap');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
