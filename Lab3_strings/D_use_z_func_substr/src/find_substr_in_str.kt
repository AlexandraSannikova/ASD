import java.io.BufferedReader
import java.io.InputStreamReader

fun main(args: Array<String>) {
    val reader = BufferedReader(InputStreamReader(System.`in`))
    val p = reader.readLine()!!
    val pLen = p.length
    val t = reader.readLine()!!
    val tLen = t.length
    val str = "$p#$t"
    val n = str.length
    val z = IntArray(n, { 0 })
    var l = 0
    var r = 0
    //z-f
    for (i in 1..n - 1) {
        z[i] = if (i < r) {
            Math.min(z[i - l], r - i)
        } else { // need it :(, because of it not i <= r
            0
        }
        while (i + z[i] < n && str[z[i]] == str[i + z[i]]) {
            z[i]++
        }
        if (i + z[i] > r) {
            l = i
            r = i + z[i]
        }
    }
    //end of calc of z-f
    val ans = arrayListOf<Int>()
    for (i in 0..tLen - 1) {
        if (z[i + pLen + 1] == pLen) {
            ans.add(i + 1)
        }
    }
    ans.sort();
    print("${ans.size}\n${ans.joinToString(" ")}") // size and ans
}