
private class TireNode {
    private boolean end = false; //是不是关键词的结尾
 
    private Map<Character, TireNode> subNodes = new HashMap<>(); //当前节点下所有的子节点
 
    public void addSubNode(Character key, TireNode node) { //添加节点
        subNodes.put(key, node);
    }
 
    public TireNode getSubNode(Character key) { //得到节点
        return subNodes.get(key);
    }
 
    public boolean isKeywordEnd() { //是否指向敏感词的最后一个字符
        return end;
    }
 
    public void setkeywordEnd(boolean end) { //设置新添加的敏感词最后一个字符为end
        this.end = end;
    }

    //增加敏感词
private void addWord(String lineTxt) {
    TireNode tempNode = rootNode;
    for (int i = 0;i < lineTxt.length();i++) {
        Character c = lineTxt.charAt(i);
        TireNode node = tempNode.getSubNode(c);
        if (node == null) {
            node = new TireNode();
            tempNode.addSubNode(c, node);
        }
        tempNode = node;
        if (i == lineTxt.length() - 1) {
            tempNode.setkeywordEnd(true);
        }
    }
}

public String filter(String text) {
    if (StringUtils.isBlank(text)) {
        return text;
    }
    StringBuilder result = new StringBuilder();
    String replacement = "***"; //打码
    TireNode tempNode = rootNode; //从根节点出发
    int begin = 0; //从句子的第一个字符开始
    int position = 0; //指向当前位置
    while (position < text.length()) {
        char c = text.charAt(position);
        if (isSymbol(c)) {
            if (tempNode == rootNode) {
                result.append(c);
                begin++;
            }
            position++;
            continue;
        }
        tempNode = tempNode.getSubNode(c);
        if (tempNode == null) {
            result.append(text.charAt(begin));
            position = begin + 1;
            begin = position;
            tempNode = rootNode;
        } else if (tempNode.isKeywordEnd()) {
            //发现敏感词
            result.append(replacement);
            position = position + 1;
            begin = position;
            tempNode = rootNode;
        } else {
            position++;
        }
    }
    result.append(text.substring(begin));
    return result.toString();
}


public static void main(String[] args) {
    SensitiveService s = new SensitiveService();
    s.addWord("色情");
    s.addWord("赌博");
    System.out.println(s.filter("i色情，hello，去赌博吗？"));
}
