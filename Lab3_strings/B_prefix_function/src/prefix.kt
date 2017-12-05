import java.io.BufferedReader
import java.io.InputStreamReader

fun main(args: Array<String>) {
    val reader = BufferedReader(InputStreamReader(System.`in`))
    val str: String = reader.readLine()!!
    val n = str.length
    val prefix = IntArray(n + 1, { 0 })
    prefix[0] = -1
    prefix[1] = 0
    var k = 0
    for(i in 2..n){
        while(k >= 0 && str[k] != str[i - 1]){
            k = prefix[k]
        }
        prefix[i] = ++k
    }
    print(prefix.drop(1).joinToString(" ")) //drop = without first element
}
