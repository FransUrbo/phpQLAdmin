<?php
// Get all database suffixes
$database_contexts = pql_get_status($_pql->ldap_linkid, "cn=Databases,cn=Monitor", 'namingContexts');
?>
  <span class="title1"><?=$LANG->_('LDAP Server Database Status')?></span>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Entry Cache Usage')?>
      <tr>
        <td class="title3"><?=$LANG->_('Suffix')?></td>
        <td class="title3"><?=$LANG->_('Type')?></td>
      </tr>

<?php if(is_array($database_contexts)) {
		for($i=0; $database_contexts[$i]; $i++) {
			// Get all information about this database/suffix
			$database_status = pql_get_status_databases($_pql->ldap_linkid, $database_contexts[$i]);
			foreach($database_status as $status) {
?>
      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?=$database_status['namingcontexts']?></td>
        <td class="<?=$class?>"><?=$database_status['description']?></td>
      </tr>
<?php		}
		}
	  } else {
		  // Get all information about this database/suffix
		  $database_status = pql_get_status_databases($_pql->ldap_linkid, $database_contexts);
		  foreach($database_status as $status) {
?>
      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?=$status['namingcontexts']?></td>
        <td class="<?=$class?>"><?=$status['description']?></td>
      </tr>
<?php	  }
	  }
?>
    </th>
  </table>
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
