<?php
// Manage the radiusFilterID attribute
// $Id: attrib.radiusfilterid.inc,v 2.1 2006-12-02 13:06:50 turbo Exp $

// {{{ attribute_check(void)
function attribute_check() {
  global $LANG, $error_text;

  $error = false;
  
  if(($_REQUEST["attrib"] == 'radiusfilterid') and
	 (!@$_REQUEST['radius_version'] or !@$_REQUEST['radius_policy']))
  {
	  $error = true;
	  if(!@$_REQUEST['radius_version'])
		$error_text['radius_version'] = $LANG->_('Missing');
	  if(!@$_REQUEST['radius_policy'])
		$error_text['radius_policy'] = $LANG->_('Missing');
  }

  return($error);
}
// }}}

// {{{ attribute_print_form()
function attribute_print_form() {
  global $_pql, $LANG, $error_text;

  $host_reference = pql_get_attribute($_pql->ldap_linkid, $_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));

  $attrib = $_REQUEST["attrib"];
  if(@$_REQUEST[$attrib]) {
	$_REQUEST[$attrib] = preg_replace('/\\\"/', '', $_REQUEST[$attrib]);

	// Extract version
	$tmp = preg_replace('/.*version=/', '', $_REQUEST[$attrib]);
	$_REQUEST[radius_version] = preg_replace('/:.*/', '', $tmp);

	// ... and policy
	$_REQUEST[radius_policy] = preg_replace('/.*policy=/', '', $_REQUEST[$attrib]);
  } else {
	// No previous value, suggest some
	$_REQUEST[radius_version] = '1';
	$_REQUEST[radius_policy]  = 'Enterprise User';
  }
?>
    <span class="title1"><?php echo pql_complete_constant($LANG->_('Change %what% for host %host%'), array('what' => $attrib, 'host' => $host_reference)); ?></span>

    <br><br>

    <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
      <table cellspacing="0" cellpadding="3" border="0">
        <th colspan="3" align="left">
          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_('RADIUS Version')?></td>
            <td>
              <?php echo pql_format_error_span($error_text['radius_version']) . "\n"; ?>
              <input type="text" name="radius_version" value="<?=$_REQUEST[radius_version]?>" size="25">
            </td>
          </tr>

          <tr class="<?php pql_format_table(); ?>">
            <td class="title"><?=$LANG->_('RADIUS Policy')?></td>
            <td>
              <?php echo pql_format_error_span($error_text['radius_policy']) . "\n"; ?>
              <input type="text" name="radius_policy" value="<?=$_REQUEST[radius_policy]?>" size="25">
            </td>
          </tr>
        </th>
      </table>

      <input type="hidden" name="submit"   value="1">
      <input type="hidden" name="oldvalue" value="<?=$_REQUEST[$attrib]?>">
      <input type="hidden" name="attrib"   value="<?=$attrib?>">
      <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
      <input type="hidden" name="host"     value="<?=$_REQUEST["host"]?>">
      <br>
      <input type="submit" value="<?=$LANG->_('Save')?>">
    </form>
  </body>
</html>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
  global $_pql, $LANG;

  $newvalue = '"Enterasys:version='.$_REQUEST['radius_version'].':policy='.$_REQUEST['radius_policy'].'"';

  if(!@$_REQUEST[oldvalue])
	$_REQUEST[$attrib] = '<i>NULL</i>';
	 
  if(pql_modify_attribute($_pql->ldap_linkid, $_REQUEST["host"], $_REQUEST["attrib"], 1, $newvalue)) {
	$msg = pql_complete_constant($LANG->_('Successfully changed %what% from %old% to %new%'),
	array('what' => $_REQUEST["attrib"],
		  'old'  => $_REQUEST["oldvalue"],
		  'new'  => $newvalue));
  } else
	$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
								 array('what' => $_REQUEST['attrib'])) . ": " . pql_format_error(0);
	
	attribute_forward($msg);
// }}}
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
