<?php
// control attribute plugin for
// maxrcptcount (maximum number of RCPT TO's)
//
// $Id: attrib.control.maxrcptcount.inc,v 2.6 2003-05-06 07:52:59 turbo Exp $

function maxrcptcount_check($type) {
	global $error, $maxrcptcount;

	// missing
	if($maxrcptcount == ""){
		$error["maxrcptcount"] = PQL_LANG_MISSING;
		return false;
	}

	// number ?
	if(preg_match ("/[^0-9]/", $maxrcptcount)){
		$error["maxrcptcount"] = PQL_LANG_INVALID;
		return false;
	}

	return true;
}

function maxrcptcount_init($host) {
	// init values
	global $_pql_control, $maxrcptcount;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									   "maxrcptcount");

	if(!is_null($value)){
		$maxrcptcount = $value[0];
	}
}

function maxrcptcount_print_view($host) {
	global $_pql_control, $maxrcptcount;

	// init data
	maxrcptcount_init($host);

	// set empty field to "not set"
	if($maxrcptcount == "" or $maxrcptcount == 0){
		$maxrcptcount = "unlimited";
 	}
?>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">maxrcptcount (Maximum number of RCPT TO's)</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Max RCPT TO's</td>
        <td><?=$maxrcptcount?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=maxrcptcount"><img src="images/edit.png" width="12" height="12" border="0">Change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function maxrcptcount_print_form() {
	global $attrib, $maxrcptcount, $PHP_SELF, $error, $mxhost;

?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">maxrcptcount (Maximum number of RCPT TO's)</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Max RCPT TO's</td>
          <td><?php echo format_error($error["maxrcptcount"]); ?><input type="text" name="maxrcptcount" value="<?=$maxrcptcount?>"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2"><img src="images/info.png" width="16" height="16" border="0">Set to 0 to allow unlimited RCPT TO's</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="mxhost" value="<?=$mxhost?>">
    <input type="submit" value="<?=PQL_LANG_SAVE?>">
  </form>
<?php
}

function maxrcptcount_save($type, $host) {
	global $_pql_control, $attrib, $maxrcptcount, $error;

	// recontrol values
	if(!maxrcptcount_check($type)){
 		return false;
	}

	switch($type){
		case "modify";
		case "add";
			// save entries
			if(pql_control_replace_attribute($_pql_control->ldap_linkid,
											 "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
											 "maxrcptcount", $maxrcptcount)){
				$msg = "Successfully saved maxrcptcount options";
			} else {
				$msg = "Failed saving maxrcptcount: " . pql_ldap_error($_pql_control->ldap_linkid);
			}

			attribute_forward($msg);
			break;
		default:
			die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
