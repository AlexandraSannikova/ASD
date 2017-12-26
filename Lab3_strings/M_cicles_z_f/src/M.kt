import java.io.File //WORK WITH FILE

fun main(args: Array<String>) {
    val input = File("cyclic.in").bufferedReader()
    val out = File("cyclic.out").bufferedWriter()
    var str: String = input.readLine()!!
    str = str.plus(str)
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
    var count_before = 0
    val n_2 = n / 2
    for (j in 1..n_2) {
        if (z[j] < n_2 && str[z[j]] > str[j + z[j]]) {
            count_before++
        }
    }
    out.append((count_before + 1).toString())
    out.close()
}