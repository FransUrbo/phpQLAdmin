<?php if(pql_get_define("PQL_GLOB_CONTROL_USE")) {
		// ------------------------------
		// These are the domains we have to check
		$domainnames = $additionaldomainname; $domainnames[] = $defaultdomain;

		// These are the attributes we have to check
		$attributes = array('locals', 'rcpthosts', 'smtproutes');

		// ------------------------------
		foreach($attributes as $attrib) {
			foreach($domainnames as $d) {
				if($attrib == 'smtproutes')
				  // Special circumstances - An smtproute isn't just the domain, so we need a wildcard.
				  $val = $d."*";
				else
				  $val = $d;

				if(pql_control_search_attribute($_pql_control->ldap_linkid, $USER_SEARCH_DN_CTR, $attrib, $val)) {
					$DEFINED[$attrib][$d] = $LANG->_('Yes');

					$alt = pql_complete_constant($LANG->_('Remove %domain% from %attribute%'),
												 array('domain' => $d, 'attribute' => $attrib));
						
					$LINKS[$attrib][$d] = "<a href=\"control_edit_attribute.php?attrib=$attrib&rootdn=$rootdn&domain=$domain&type=del&set=$d&submit=1&view=$view\"><img src=\"images/del.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"$alt\"></a>";
				} else {
					$DEFINED[$attrib][$d] = $LANG->_('No');

					$alt = pql_complete_constant($LANG->_('Add %what% to %attribute%'),
												 array('what' => $d, 'attribute' => $attrib));
					
					$LINKS[$attrib][$d] = "<a href=\"control_edit_attribute.php?attrib=$attrib&rootdn=$rootdn&domain=$domain&type=add&set=$d&view=$view\"><img src=\"images/edit.png\" width=\"12\" height=\"12\" border=\"0\" alt=\"$alt\"></a>";
				}
			}
		}
?>

<?php	foreach($attributes as $attrib) { ?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=$LANG->_('Control options')?>: <?php echo pql_complete_constant($LANG->_('Defined in %attribute%'), array('attribute' => $attrib)); ?>
<?php		foreach($domainnames as $d) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?=$d?></td>
        <td><?=$DEFINED[$attrib][$d]?></td>
        <td><?=$LINKS[$attrib][$d]?></td>
      </tr>

<?php		} ?>
    </th>

  </table>
  <br>
<?php	} ?>

  <br><br>

<?php } // end if(PQL_GLOB_CONTROL_USE) ...
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
