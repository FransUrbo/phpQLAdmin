  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">{$title}<br>
{foreach item=line from=$show}
      <tr class="{php}echo pql_format_table(){/php}">
        <td class="title">{$line.title}</td>
        <td>{$line.value}</td>
        <td>{$line.link}</td>
      </tr>
{/foreach}
