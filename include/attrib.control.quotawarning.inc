<?php
// control attribute plugin for
// quotawarning (additional quota warning message)
//
// $Id: attrib.control.quotawarning.inc,v 2.2 2002-12-17 12:24:35 turbo Exp $

function quotawarning_check($type) {
	global $error, $quotawarning, $use_quotawarning;

	// missing
	if($use_quotawarning == 1 and $quotawarning == ""){
		$error["quotawarning"] = PQL_MISSING;
		return false;
	}

	return true;
}

function quotawarning_init($host) {
	// init values
	global $_pql_control, $quotawarning, $use_quotawarning;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   "cn=" . $host . "," . $USER_SEARCH_DN_CTR,
									   "quotawarning");

	if(!is_null($value)){
		$quotawarning = $value[0];
	}

	// initialize radio button
	if($quotawarning == ""){
		$use_quotawarning = 0;
 	} else {
		$use_quotawarning = 1;
  }
}

function quotawarning_print_view($host) {
	global $_pql_control, $quotawarning, $use_quotawarning;
	
	// init data
	quotawarning_init($host);
	
	if($use_quotawarning == "0"){
		$quotawarning = "disabled";
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">quotawarning (Additional quota warning message)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Message</td>
        <td><?php echo nl2br($quotawarning); ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?attrib=quotawarning"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function quotawarning_print_form() {
	global $attrib, $quotawarning, $use_quotawarning,  $PHP_SELF, $error;

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">quotawarning (Additional quota warning message)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Send quota warnings</td>
          <td>
            <input type="radio" name="use_quotawarning" value="0" <?php if($use_quotawarning == "0"){ echo "checked"; } //" ?>> no
            <input type="radio" name="use_quotawarning" value="1" <?php if($use_quotawarning == "1"){ echo "checked"; } //" ?>> yes
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Message</td>
          <td><?php echo format_error($error["quotawarning"]); ?><textarea cols="40" rows="10" name="quotawarning"><?php echo $quotawarning; ?></textarea></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="submit" value="<?php echo PQL_SAVE; ?>">
  </form>

<?php
}

function quotawarning_save($type, $host) {
	global $_pql_control, $attrib, $quotawarning, $use_quotawarning, $error;

	// recontrol values
	if(!quotawarning_check($type)){
 		return false;
	}

	switch($type){
	  case "modify";
		// save entries
		if($use_quotawarning == "0"){
			$quotawarning = "";
		}
		
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 "cn=" . $host . "," . $USER_SEARCH_DN_CTR,
										 "quotawarning", $quotawarning)){
			$msg = "Successfully saved quotawarning message";
		} else {
			$msg = "Failed saving quotawarning message: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function quotawarning_help(){
	// help text taken from
	// life with qmail-ldap (http://www.lifewithqmail.org/ldap/)
	?>
	Custom text in quota warning message. If you set to no, no quota warning
	messages will be issued
	<?php
}

function quotawarning_help_cr(){
?>
	help text taken from life with qmail-ldap
<?php
}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
