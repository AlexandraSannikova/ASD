import java.io.BufferedReader
import java.io.InputStreamReader

fun main(args: Array<String>) {
    val reader = BufferedReader(InputStreamReader(System.`in`))
    val str: String = reader.readLine()!!
    val n = str.length
    val z = IntArray(n, { 0 })
    var l = 0
    var r = 0
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
    print(z.drop(1).joinToString(" "))
}